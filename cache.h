#ifndef CACHE_H_
#define CACHE_H_

#include "common.h"
#include "searcher.h"
#include "tst-line.h"
#include <stack>

class Cache {
public:
  Cache();
  ~Cache();
  Searcher* generate();
  Searcher* fetch();
  void push(Searcher*);
  void pop();

private:
  stack<Searcher*> cache;
};

#endif
