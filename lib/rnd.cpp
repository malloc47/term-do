#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <sstream>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "plugin.h"
#include "plug.h"

using namespace std;

class Rnd : public Plugin {
public:
  string name() {return "rnd";}
  void update(list_t new_tokens) {tokens = new_tokens; srand(time(NULL));}
  bool match() {return false;}
  list_t list() {
    list_t output;
    for(int i=0;i<rand()%100;i++)
      output.push_back(randomStrGen(rand()%20));
    return output;
  }
  string cmd() {return "";}

private:
  list_t tokens;
  // http://stackoverflow.com/questions/2146792/how-do-you-generate-random-strings-in-
  // only using a fixed version, obviously...
  string randomStrGen(int length) {
    static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    string result;
    result.resize(length);
    for (int i = 0; i < length; i++)
      result[i] = charset[rand() % charset.length()];
    return result;
  }
};

extern "C" Plugin* create_plugin() {return new Rnd;}
extern "C" void destroy_plugin( Plugin* plugin ) {delete plugin;}
