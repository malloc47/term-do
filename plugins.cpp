#include "plugins.h"

Plugins::Plugins() {
  list_t libraries = findLibraries("./lib");
  FOR_l(i,libraries)
      loadLibrary("./lib/" + libraries[i]);
}

Plugins::Plugins(list_t libraries) {
  FOR_l(i,libraries)
      loadLibrary("./lib/" + libraries[i]+".so");
}

Plugins::~Plugins() {
  FOR_l(i,plugins) {
    plugins[i].destroy(plugins[i].obj);
    dlclose(plugins[i].handle);
  }
}

// Directory listing of *.so files
list_t Plugins::findLibraries(string dirname) {
  list_t output;
  struct dirent *entry;
  DIR* dirp = opendir(dirname.c_str());
  if(dirp==NULL) {
    cout << "\rInvalid plugin directory\n";
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
    cout << "\rFailed to open library " << dlerror() << "\n";
    return false;
  }

  dlerror();
  plugin.create  = (create_f) dlsym(plugin.handle, "create_plugin");
  if (dlerror()) {
    cout << "\rFailed to load \"create_plugin\" symbol\n";
    dlclose(plugin.handle);
    return false;
  }

  dlerror();
  plugin.destroy  = (destroy_f) dlsym(plugin.handle, "destroy_plugin");
  if (dlerror()) {
    cout << "\rFailed to load \"destroy_plugin\" symbol\n";
    dlclose(plugin.handle);
    return false;
  }

  plugin.obj=plugin.create();
  plugins.push_back(plugin);

  return true;
}

void Plugins::populate(Searcher* searcher) {
  // exclusive means that a plugin wants a 'lock' on the matches, so
  // that only relevant matches are shown
  bool exclusive = false;
  FOR_l(i,plugins)
    if(plugins[i].obj->match())
      exclusive = true;

  FOR_l(i,plugins) {
    // if one or more plugins have requested exclusive mode, then only
    // show those completions
    if(exclusive && !plugins[i].obj->match()) continue;
    list_t dict = plugins[i].obj->list();
    if(!dict.empty())
      FOR_l(j,dict)
	searcher->insert(dict[j]);
  }

  // if in exclusive mode, see if the plugin specifies another plugin
  // for completion, find that plugin, and add the completions in
  if(exclusive)
    FOR_l(i,plugins)
      if(plugins[i].obj->match() && !plugins[i].obj->complete().empty()) {
	string comp = plugins[i].obj->complete();
	string name;
	// Allow "arguments" from one plugin to another
	string arg = "";
	if(comp.find(':') != string::npos) {
	  stringstream ss(comp);
	  getline(ss,name,':');
	  getline(ss,arg);
	}
	else {
	  name = comp;
	}
	FOR_l(j,plugins)
	  if(j!=i && !plugins[j].obj->name().compare(name)) {
	    list_t dict = plugins[j].obj->list(arg);
	    FOR_l(j,dict)
	      searcher->insert(dict[j]);
	  }
      }
}

void Plugins::populateAll(Searcher* searcher) {
  FOR_l(i,plugins) {
    list_t dict = plugins[i].obj->list();
    if(!dict.empty())
      FOR_l(j,dict)
	searcher->insert(dict[j]);
  }
}

string Plugins::getCommand() {
  // TODO: handle collisions
  FOR_l(i,plugins) {
    string cmd = plugins[i].obj->cmd();
    if(!cmd.empty())
      return cmd;
  }
  return "";
}

void Plugins::update(list_t tokens) {
  FOR_l(i,plugins)
    plugins[i].obj->update(tokens);
}
