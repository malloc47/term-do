#include "tokens.h"

Tokens::Tokens() {
  cache = new Cache<Searcher*>();
  Searcher *s = new SEARCHER_t();
  cache->push(s);
}
Tokens::~Tokens() {delete cache;}

list_t Tokens::getTokens() {return tokens;}

Searcher* Tokens::push(string token) {
  tokens.push_back(token);
  Searcher *s = new TST();
  cache->push(s);
  return s;
}

Searcher* Tokens::pop() {
  if(tokens.size() > 0) {
    tokens.pop_back();
    cache->pop();
  }
  return cache->top();
}
Searcher* Tokens::getSearcher() {return cache->top();}
