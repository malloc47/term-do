#include "cache.h"

Cache::Cache() {}

Cache::~Cache() {
  while(!cache.empty()) {
    delete cache.top();
    cache.pop();
  }
}

Searcher* Cache::generate() {
  Searcher* s = new TST();
  push(s);
  return s;
}

Searcher* Cache::fetch() {
  if(!cache.empty())
    return cache.top();
  else
    return generate();
}

void Cache::push(Searcher* s) {cache.push(s);}
void Cache::pop() {
  delete cache.top(); 
  cache.pop();
}
