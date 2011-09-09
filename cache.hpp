#ifndef CACHE_HPP_
#define CACHE_HPP_

#include "common.h"
#include "searcher.h"
#include "tst-line.h"
#include <stack>

template <class T>
class Cache {
public:
  Cache();
  ~Cache();
  T generate();
  T fetch();
  void push(T);
  void pop();

private:
  stack<T> cache;
};

template <class T>
Cache<T>::Cache() {}

template <class T>
Cache<T>::~Cache() {
  while(!cache.empty()) {
    delete cache.top();
    cache.pop();
  }
}

template <class T>
T Cache<T>::generate() {
  T s = new TST();
  push(s);
  return s;
}

template <class T>
T Cache<T>::fetch() {
  if(!cache.empty())
    return cache.top();
  else
    return generate();
}

template <class T>
void Cache<T>::push(T s) {cache.push(s);}

template <class T>
void Cache<T>::pop() {
  delete cache.top(); 
  cache.pop();
}


#endif
