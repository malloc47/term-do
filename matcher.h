#ifndef MATCHER_H_
#define MATCHER_H_

#include "common.h"
#include "searcher.h"
#include "tst.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

class Matcher{
public:
  Matcher();
  ~Matcher();
  void setDictionary(list_t);
  void addCharRestricted(char);
  void addChar(char);
  void insert(string); // stub until we start passing around TSTs instead of matchers
  bool removeChar();
  list_t getMatches();
  string getMatch();
  bool exactMatch();
  string getQuery();
  void rotateForward();
  void rotateBackward();

private:
  Searcher *searcher;
  string query;
  list_t dictionary;
  // for better or worse, matches are buffered in the history stack,
  // which is a space for time tradeoff that reduce number of search
  // operatons when backspacing characters
  stack< list_t > history;
};

#endif
