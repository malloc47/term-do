#ifndef TST_H_
#define TST_H_

#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <fstream>

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

class TST {
public:
  TST();
  ~TST();
  void insert(string s);
  bool search(string s);
  bool rsearch(string s);
  vector<string> prefixSearch(string s);
  vector<string> hammingSearch(string s,int d);
  vector<string> sort();
private:
  TSTp root;
  bool rsearch(TSTp p,string *s, unsigned int pos);
  TSTp insert(TSTp p, string *s, unsigned int pos);
  void prefixSearch(TSTp p,string *s, unsigned int pos, vector<string> *output);
  void hammingSearch(TSTp p, string *s, unsigned int pos, int d, vector<string> *output);
  void sort(TSTp p,vector<string> *output);
  void cleanup(TSTp p);
  char downcase(char);
};

#endif
