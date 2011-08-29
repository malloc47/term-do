#ifndef MATCHER_H_
#define MATCHER_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
//#include <algorithm>

using namespace std;

class Matcher {
public:
  Matcher();
  void wordAdd(char);
  void wordRemove();
  vector<string> wordMatches();
  string wordCurrent();

private:
  void init();
  string prompt;
  string word;
  string command;
  vector<string> words;
  vector<string> actions;
};

#endif
