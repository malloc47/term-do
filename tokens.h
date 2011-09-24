#ifndef TOKENS_H_
#define TOKENS_H_

#include "common.h"
#include "searcher.h"
#include "tst.h"
#include "tst-line.h"
#include "cache.hpp"

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
  list_t& getTokens();
  Searcher* push(string);
  Searcher* pop();
  Searcher* getSearcher();
  // bool commitToken(string);
  // bool commitValidToken(string);
  // bool commitFinalToken(string);

private:
  Cache<Searcher*>* cache;
  list_t tokens;
};

#endif
