#include "verbs.h"

Verbs::Verbs() {
  loadLibrary("./lib/file.so");
  list_t input;
  list_t test = plugins[0](input);
  for(unsigned int i=0; i<test.size();i++)
    printf("%s ",test.at(i).c_str());
}

Verbs::~Verbs() {
  // cleanup handles
  while(!handles.empty()) {
    dlclose(handles.top());
    handles.pop();
  }
}

bool Verbs::loadLibrary(string library) {
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

vector<string> Verbs::getVerbs() {
  // return plugin1(tokens);
  vector<string> dictionary;
  dictionary.push_back("vgexport");
  dictionary.push_back("vgscan");
  dictionary.push_back("vmstat");
  dictionary.push_back("vpxenc");
  return dictionary;
}

vector<string> Verbs::getTokens() {return tokens;}

void Verbs::push(string token) {tokens.push_back(token);}

void Verbs::pop() {
  if(tokens.size() > 0)
    tokens.pop_back();
}
