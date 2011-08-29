#ifndef TERMDO_H_
#define TERMDO_H_

#include "vt100.h"
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
  void loopDo();

private:
  void init();
  int handleChar(char);
  void refreshLine();
  string wordMatches();
  void wordAdd(char);
  void wordRemove();
  string prompt;
  string word;
  string command;
  vector<string> words;
  vector<string> actions;
  class VT100 term_control;
};

#endif
