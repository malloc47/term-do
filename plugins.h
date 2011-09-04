#ifndef PLUGINS_H_
#define PLUGINS_H_

#include "common.h"
#include "matcher.h"
#include <dlfcn.h>
#include <dirent.h>
#include <vector>
#include <stack>
#include <string>
#include <stdarg.h>
#include <stdio.h>

using namespace std;

class Plugins {
public:
  Plugins();
  ~Plugins();
  void getDictionary(Matcher*);
  bool unambiguousCommand();
  string getCommand();
  list_t getTokens(void);
  void pop();
  void push(string);
  bool loadLibrary(string);
  list_t findLibraries(string);
private:
  list_t tokens;
  vector<plugin_t> plugins;
};

#endif