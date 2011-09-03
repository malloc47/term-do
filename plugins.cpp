#include "plugins.h"

Plugins::Plugins() {
  list_t libraries = findLibraries("./lib");
  FOR_l(i,libraries)
      loadLibrary("./lib/" + libraries[i]);
}

Plugins::~Plugins() {
  FOR_l(i,plugins)
    dlclose(plugins[i].handle);
}

// Directory listing of *.so files
list_t Plugins::findLibraries(string dirname) {
  list_t output;
  struct dirent *entry;
  DIR* dirp = opendir(dirname.c_str());
  if(dirp==NULL) {
    printf("Invalid plugin directory\n");
    return output;
  }
  while ((entry = readdir(dirp)) != NULL) {
    string name(entry->d_name);
    if(name.size() > 3 && name.compare(name.size()-3,3,".so") == 0)
      output.push_back(name);
  }
  closedir(dirp);
  return output;
}

bool Plugins::loadLibrary(string library) {

  plugin_t plugin;  

  plugin.handle = dlopen(library.c_str(), RTLD_LAZY);
  if (!plugin.handle) {
    printf( "Failed to open library: %s\n",dlerror());
    return false;
  }

  dlerror();
  plugin.list  = (list_f) dlsym(plugin.handle, "list");
  if (dlerror()) {
    printf("Failed to load \"list\" symbol");
    dlclose(plugin.handle);
    return false;
  }

  dlerror();
  plugin.update  = (update_f) dlsym(plugin.handle, "update");
  if (dlerror()) {
    printf("Failed to load \"update\" symbol");
    dlclose(plugin.handle);
    return false;
  }

  dlerror();
  plugin.cmd  = (cmd_f) dlsym(plugin.handle, "cmd");
  if (dlerror()) {
    printf("Failed to load \"cmd\" symbol");
    dlclose(plugin.handle);
    return false;
  }

  plugins.push_back(plugin);
  return true;
}

void Plugins::getDictionary(Matcher* matcher) {
  FOR_l(i,plugins) {
    list_t dict = plugins[i].list();  
    if(!dict.empty())
      FOR_l(j,dict)
	matcher->insert(dict[j]);
  }
}

bool Plugins::unambiguousCommand() {
  return tokens.size() > 2;
}

string Plugins::getCommand() {
  return plugins[0].cmd();
  // if(tokens.empty()) return "";
  // else return tokens.front();
}

list_t Plugins::getTokens() {return tokens;}

void Plugins::push(string token) {
  tokens.push_back(token);
  // update individual plugins
  FOR_l(i,plugins) {
    plugins[i].update(tokens);
  }
}

void Plugins::pop() {
  if(tokens.size() > 0) {
    tokens.pop_back();
    // update individual plugins
    FOR_l(i,plugins) {
      plugins[i].update(tokens);
    }
  }
}
