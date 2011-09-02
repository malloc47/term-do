#ifndef MATCHER_H_
#define MATCHER_H_

#include "common.h"
#include "tst.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

class Matcher : public TST{
public:
  Matcher();
  ~Matcher();
  void setDictionary(list_t);
  void addCharRestricted(char);
  void addChar(char);
  bool removeChar();
  list_t getMatches();
  string getMatch();
  bool exactMatch();
  string getQuery();
  void rotateForward();
  void rotateBackward();

private:
  list_t matches(string);
  string query;
  list_t dictionary;
  stack< list_t > history;
};

#endif
