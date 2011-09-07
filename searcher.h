// No references to John Wayne please
#ifndef SEARCHER_H_
#define SEARCHER_H_

#include "common.h"

// Abstract class for any object to do string searching
class Searcher {
public:
  virtual void insert(string s) = 0;
  virtual bool search(string s) = 0;
  virtual list_t searchp(string s) = 0;
  virtual bool containsp(string s) = 0;
};


#endif
