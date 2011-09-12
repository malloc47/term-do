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
    printf( "\rFailed to open library %s\n",dlerror());
    return false;
  }

  dlerror();
  plugin.create  = (create_f) dlsym(plugin.handle, "create_plugin");
  if (dlerror()) {
    printf("\rFailed to load \"create_plugin\" symbol");
    dlclose(plugin.handle);
    return false;
  }

  dlerror();
  plugin.destroy  = (destroy_f) dlsym(plugin.handle, "destroy_plugin");
  if (dlerror()) {
    printf("\rFailed to load \"destroy_plugin\" symbol");
    dlclose(plugin.handle);
    return false;
  }

  plugin.obj=plugin.create();
  plugins.push_back(plugin);

  return true;
}

void Plugins::populate(Searcher* searcher) {
  bool exclusive = false;
  FOR_l(i,plugins)
    if(plugins[i].obj->match())
      exclusive = true;

  FOR_l(i,plugins) {
    if(exclusive && !plugins[i].obj->match()) continue;
    list_t dict = plugins[i].obj->list();
    // if(dict.size()==1 && dict[0].size() > 1 && 
    //    dict[0][0] == '%' && dict[0][dict[0].size()-1] == '%')
    //   cross_ref.push_back(dict[0].substr(1,dict[0].size()-2));
    // else
    if(!dict.empty())
      FOR_l(j,dict)
	searcher->insert(dict[j]);
  }

  if(exclusive)
    FOR_l(i,plugins)
      if(!plugins[i].obj->match() && !plugins[i].obj->complete().empty())
	FOR_l(j,plugins)
	  if(j!=i && !plugins[j].obj->name().compare(plugins[i].obj->complete())) {
	    list_t dict = plugins[j].obj->list();
	    FOR_l(j,dict)
	      searcher->insert(dict[j]);
	  }
}

void Plugins::populateAll(Searcher* searcher) {
  FOR_l(i,plugins) {
    list_t dict = plugins[i].obj->list();
    // if(!(dict.size()==1 && dict[0].size() > 1 && 
    // 	 dict[0][0] == '%' && dict[0][dict[0].size()-1] == '%'))
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
