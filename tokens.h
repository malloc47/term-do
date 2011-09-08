#ifndef TOKENS_H_
#define TOKENS_H_

#include "common.h"
#include "searcher.h"
#include "tst.h"
#include "tst-line.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

class Tokens{
public: 
  Tokens();
  ~Tokens();
  list_t getTokens();
  void push(string);
  void pop();
  // bool commitToken(string);
  // bool commitValidToken(string);
  // bool commitFinalToken(string);

private:
  list_t tokens;
};

#endif
