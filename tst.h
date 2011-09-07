#ifndef TST_H_
#define TST_H_

#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include "searcher.h"
#include "common.h"

using namespace std;

typedef struct TSTNode *TSTp; 

class TSTNode {
public:
  TSTNode(char);
  ~TSTNode();
  char val;
  TSTp left, middle, right; 
  string *data;
private:
};

class TST : public Searcher {
public:
  TST();
  ~TST();
  void insert(string s);
  bool search(string s);
  bool searchr(string s);
  list_t searchp(string s);
  bool containsp(string s);
  list_t searchh(string s,int d);
  list_t sort();
private:
  TSTp root;
  bool searchr(TSTp p,string *s, unsigned int pos);
  TSTp insert(TSTp p, string *s, unsigned int pos);
  void searchp(TSTp p,string *s, unsigned int pos, list_t *output);
  bool containsp(TSTp p,string *s, unsigned int pos);
  void searchh(TSTp p, string *s, unsigned int pos, int d, list_t *output);
  void sort(TSTp p,list_t *output);
  void cleanup(TSTp p);
  char downcase(char);
};

#endif
