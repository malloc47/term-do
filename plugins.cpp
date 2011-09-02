#include "plugins.h"

Plugins::Plugins() {
  list_t libraries = findLibraries("./lib");
  FOR_l(i,libraries)
      loadLibrary("./lib/" + libraries[i]);
  list_t input;
  list_t test = plugins[0](input);
}

Plugins::~Plugins() {
  // cleanup handles
  while(!handles.empty()) {
    dlclose(handles.top());
    handles.pop();
  }
}

// Directory listing of *.so files
list_t Plugins::findLibraries(string dirname) {
  list_t output;
  struct dirent *entry;
  DIR* dirp = opendir(dirname.c_str());
  if(dirp==NULL) printf("Invalid plugin directory\n");
  while ((entry = readdir(dirp)) != NULL) {
    string name(entry->d_name);
    if(name.size() > 3 && name.compare(name.size()-3,3,".so") == 0)
      output.push_back(name);
  }
  closedir(dirp);
  return output;
}


bool Plugins::loadLibrary(string library) {
  void* dyn_handle = dlopen(library.c_str(), RTLD_LAZY);
  if (!dyn_handle) {
    printf( "Failed to open library: %s\n",dlerror());
    return false;
  }
  dlerror();

  list_f list  = (list_f) dlsym(dyn_handle, "list");
  const char *error = dlerror();
  if (error) {
    printf( "Failed to load symbol list: %s\n",error);
    dlclose(dyn_handle);
    return false;
  }
  
  plugins.push_back(list);
  handles.push(dyn_handle);
  return true;
}

void Plugins::getDictionary(Matcher* matcher) {
  list_t dict = plugins[0](tokens);  
  FOR_l(i,dict)
    matcher->insert(dict[i]);
}

bool Plugins::unambiguousCommand() {
  return tokens.size() > 2;
}

string Plugins::getCommand() {
  if(tokens.empty()) return "";
  else return tokens.front();
}

list_t Plugins::getTokens() {return tokens;}

void Plugins::push(string token) {tokens.push_back(token);}

void Plugins::pop() {
  if(tokens.size() > 0)
    tokens.pop_back();
}
