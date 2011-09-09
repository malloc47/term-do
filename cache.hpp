#ifndef CACHE_HPP_
#define CACHE_HPP_

#include "common.h"
#include "searcher.h"
#include "tst-line.h"
#include <stack>

template <class T>
class Cache {
public:
  ~Cache() {
    while(!cache.empty())
      cache.pop();
  }
  T top() {return cache.top();}
  void push(T s) {cache.push(s);}
  void pop() {cache.pop();}
private:
  stack<T> cache;
};

// Specialization that handles pointers (hence the new/delete constructor/cleanup)
template <class T>
class Cache<T*> {
public:
  ~Cache() {
    while(!cache.empty()) {
      delete cache.top();
      cache.pop();
    }
  }
  T* top() {return cache.top();}
  void push(T* s) {cache.push(s);}
  void pop() {
    delete cache.top(); 
    cache.pop();
  }
private:
  stack<T*> cache;
};

#endif
