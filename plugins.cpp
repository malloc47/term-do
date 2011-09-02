#include "plugins.h"

Plugins::Plugins() {
  list_t libraries = findLibraries("./lib");
  FOR_l(i,libraries)
      loadLibrary("./lib/" + libraries[i]);
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
  void* dyn_handle = dlopen(library.c_str(), RTLD_LAZY);
  if (!dyn_handle) {
    printf( "Failed to open library: %s\n",dlerror());
    return false;
  }

  dlerror();
  list_f list_function  = (list_f) dlsym(dyn_handle, "list");
  if (dlerror()) {
    printf("Failed to load \"list\" symbol");
    dlclose(dyn_handle);
    return false;
  }

  dlerror();
  insert_f insert_function  = (insert_f) dlsym(dyn_handle, "insert");
  if (dlerror()) {
    printf("Failed to load \"insert\" symbol");
    dlclose(dyn_handle);
    return false;
  }

  dlerror();
  type_f type_function  = (type_f) dlsym(dyn_handle, "type");
  if (dlerror()) {
    printf("Failed to load \"type\" symbol");
    dlclose(dyn_handle);
    return false;
  }

  dlerror();
  cmd_f cmd_function  = (cmd_f) dlsym(dyn_handle, "cmd");
  if (dlerror()) {
    printf("Failed to load \"cmd\" symbol");
    dlclose(dyn_handle);
    return false;
  }
  
  list_functions.push_back(list_function);
  insert_functions.push_back(insert_function);
  type_functions.push_back(type_function);
  cmd_functions.push_back(cmd_function);
  handles.push(dyn_handle);
  return true;
}

void Plugins::getDictionary(Matcher* matcher) {
  FOR_l(i,list_functions) {
    list_t dict = list_functions[0]();  
    if(!dict.empty())
      FOR_l(j,dict)
	matcher->insert(dict[j]);
  }
}

bool Plugins::unambiguousCommand() {
  return tokens.size() > 2;
}

string Plugins::getCommand() {
  return cmd_functions[0]();
  // if(tokens.empty()) return "";
  // else return tokens.front();
}

list_t Plugins::getTokens() {return tokens;}

void Plugins::push(string token) {
  tokens.push_back(token);
  FOR_l(i,insert_functions) {
    insert_functions[i](token);
  }
}

void Plugins::pop() {
  if(tokens.size() > 0)
    tokens.pop_back();
}
