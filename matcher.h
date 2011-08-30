#ifndef MATCHER_H_
#define MATCHER_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
//#include <algorithm>

using namespace std;

class Matcher {
public:
  Matcher();
  void setDictionary(vector<string>);
  void addChar(char);
  bool removeChar();
  vector<string> getMatches();
  string getQuery();
  void rotateForward();
  void rotateBackward();

private:
  void init();
  vector<string> matches(string);
  string query;
  vector<string> dictionary;
  stack< vector<string> > history;
};

#endif
