#ifndef PLUGINS_H_
#define PLUGINS_H_

#include "common.h"
#include "query.h"
#include "lib/plugin.h"
#include <dlfcn.h>
#include <dirent.h>
#include <vector>
#include <stack>
#include <string>
#include <stdarg.h>
#include <stdio.h>

using namespace std;

typedef Plugin* (*create_f)();
typedef void (*destroy_f)(Plugin*);

typedef struct {
  create_f create;
  Plugin* obj;
  destroy_f destroy;
  void *handle;
} plugin_t;

class Plugins {
public:
  Plugins();
  Plugins(list_t);
  ~Plugins();
  void populate(Searcher*);
  void populateAll(Searcher*);
  string getCommand();
  // list_t getTokens(void);
  // void pop();
  // void push(string);
  void update(list_t);
  bool loadLibrary(string);
  list_t findLibraries(string);
private:
  // list_t tokens; // This should be moved to Matcher which should subsequently be name Query
  vector<plugin_t> plugins;
};

#endif
