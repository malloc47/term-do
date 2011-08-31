#ifndef VERBS_H_
#define VERBS_H_

#include <dlfcn.h>
#include <vector>
#include <string>
#include <stdarg.h>
#include <stdio.h>

using namespace std;

class Verbs {
public:
  Verbs();
  ~Verbs();
  vector<string> getVerbs(void);
  vector<string> getTokens(void);
  void pop();
  void push(string);
  void loadLibrary(string);
private:
  vector<string> tokens;
  vector<void*> handles;
};

#endif
