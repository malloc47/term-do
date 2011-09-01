#ifndef TST_H_
#define TST_H_

#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

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

typedef struct tstnode *TSTptr; 
typedef struct tstnode { 
   char val; 
   TSTptr left, middle, right; 
} TSTnode; 

class TST {
public:
  TST();
  ~TST();
  void insert(string s);
  bool search(string s);
  vector<string> hammingSearch(string s,unsigned int d);
  void traverse();
private:
  TSTp root;
  const char *str;
  TSTp insert(TSTp p, string *s, unsigned int pos);
  bool search(TSTp p, string *s, unsigned int pos);
  void hammingSearch(TSTp p, string *s, unsigned int pos, unsigned int d, vector<string> *output);
  void traverse(TSTp p);
  TSTptr insertr(TSTptr p, const char *s);
  int rsearch(TSTptr p, const char *s);
  void cleanup(TSTp p);
};

#endif
