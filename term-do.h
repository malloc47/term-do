#ifndef TERMDO_H_
#define TERMDO_H_

#include "vt100.h"
#include "matcher.h"
#include "verbs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class TermDo {
public:
  TermDo();
  TermDo(string);
  ~TermDo();
  void setPrompt(string);
  bool commitToken();
  string loopDo();

private:
  void init();
  void refreshLine();
  int handleChar(char);
  string formatMatches(vector<string>,unsigned int);
  string formatTokens(vector<string>);
  string prompt;
  int match_offset;
  class VT100 term;
  class Matcher matcher;
  class Verbs verbs;
};

#endif
