#ifndef VERBS_H_
#define VERBS_H_

#include <vector>
#include <string>
#include <stdarg.h>

using namespace std;

class Verbs {
public:
  vector<string> getVerbs(void);
  vector<string> getTokens(void);
  void pop();
  void push(string);
private:
  vector<string> tokens;
};

#endif
