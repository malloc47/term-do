#ifndef TSTLINE_H_
#define TSTLINE_H_

#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include "searcher.h"
#include "tst.h"
#include "common.h"

using namespace std;

// wrapper around a standard TST to support whole-line searching
class TSTLine : public TST {
public:
  virtual void insert(string s);
  // virtual bool search(string s);
  virtual list_t searchp(string s);
  // virtual bool containsp(string s);
private:
  list_t splitLine(const string);
  // list_t splitPath(const string);
  // string assembleLine(list_t);
  list_t lines;
};

#endif
