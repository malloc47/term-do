#ifndef VERBS_H_
#define VERBS_H_

#include "common.h"
#include <dlfcn.h>
#include <vector>
#include <stack>
#include <string>
#include <stdarg.h>
#include <stdio.h>

using namespace std;

typedef vector<string> (*list_f) ( vector<string> );

class Verbs {
public:
  Verbs();
  ~Verbs();
  vector<string> getVerbs(void);
  vector<string> getTokens(void);
  void pop();
  void push(string);
  bool loadLibrary(string);
private:
  vector<string> tokens;
  vector<list_f> plugins;
  stack<void*> handles;
};

#endif
