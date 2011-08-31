#include "verbs.h"

Verbs::Verbs() {
  void* dyn_handle = dlopen("lib/file.so", RTLD_LAZY);
  if (!dyn_handle) {
    printf( "Failed to open library: %s\n",dlerror());
  }
  dlerror();

  typedef vector<string> (*list_f) ( vector<string> );
  list_f list  = (list_f) dlsym(dyn_handle, "list");
  const char *error = dlerror();
  if (error) {
    printf( "Failed to load symbol list: %s\n",error);
    dlclose(dyn_handle);
  }
 
  vector<string> input;

  vector<string> test = list(input);

  for(unsigned int i=0; i<test.size();i++)
    printf("%s",test.at(i).c_str());

  // close the library
  dlclose(dyn_handle);
}

Verbs::~Verbs() {}

void Verbs::loadLibrary(string library) {

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
