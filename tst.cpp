#include "tst.h"

TST::TST() {root=0;}
TST::~TST() {cleanup(root);}

bool TST::search(string s) {
  transform(s.begin(),s.end(),s.begin(),::tolower);
  if(s.empty()) return false;
  TSTp p = root;
  unsigned int pos = 0;
  while(p) {
    // char c = downcase(pos < s.length() ? s[pos] : 0);
    char c = (pos < s.length() ? s[pos] : 0);
    if(c < p->val)
      p=p->left;
    else if(c == p->val) {
      if(p->data) return true;
      p=p->middle;
      pos++;
    }
    else
      p=p->right;
  }
  return false;
}

bool TST::searchr(string s) { return searchr(root,&s,0);}
bool TST::searchr(TSTp p,string *s, unsigned int pos) {
  if(!p || s->empty()) return false;
  char c = downcase(pos < s->length() ? s->at(pos) : 0);
  if(c < p->val)
    return searchr(p->left,s,pos);
  else if(c > p->val)
    return searchr(p->right,s,pos);
  else {
    if(c==0) return true;
    return searchr(p->middle,s,pos+1);
  }
}

list_t TST::searchp(string s) { 
  list_t output;
  if(s.size() > 0) searchp(root,&s,0,&output);
  else output=sort();
  return output;
}
void TST::searchp(TSTp p,string *s, unsigned int pos, list_t *output) {
  if(!p || s->empty()) return;
  char c = downcase(pos < s->length() ? s->at(pos) : 0);
  if(c < p->val)
    searchp(p->left,s,pos,output);
  else if(c > p->val)
    searchp(p->right,s,pos,output);
  else {
    if(pos==s->length()-1)
      sort(p->middle,output);
    searchp(p->middle,s,pos+1,output);
  }
}

bool TST::containsp(string s) {return containsp(root,&s,0);}
bool TST::containsp(TSTp p,string *s, unsigned int pos) {
  if(!p || s->empty()) return false;
  char c = downcase(pos < s->length() ? s->at(pos) : 0);
  if(c < p->val)
    return containsp(p->left,s,pos);
  else if(c > p->val)
    return containsp(p->right,s,pos);
  else {
     if(pos==s->length()-1)
       return true;
    return containsp(p->middle,s,pos+1);
  }
}

// Hamming distance search
list_t TST::searchh(string s,int d) {
  list_t output;
  searchh(root,&s,0,d,&output);
  return output;
}
void TST::searchh(TSTp p, string *s, unsigned int pos, int d, list_t *output) {
  if(!p || d<0) return;
  char c = downcase(pos < s->length() ? s->at(pos) : 0);
  if(d>0 || c < p->val)
    searchh(p->left,s,pos,d,output);
  if(p->val == 0) {
    // if( s->length()-pos <= d)
    if(pos >= s->length()) // no shorter words
      output->push_back(*p->data);
  }
  else
    searchh(p->middle,s,c ? pos+1:pos, (c==p->val) ? d:d-1,output);
  if(d>0 || c > p->val)
    searchh(p->right,s,pos,d,output);
}

void TST::insert(string s) {
  // Use this instead of downcase() to reduce amount of work
  transform(s.begin(),s.end(),s.begin(),::tolower);
  root = insert(root,&s,0);
}
TSTp TST::insert(TSTp p, string *s, unsigned int pos) {
  if(s->empty()) return p;
  // char c = downcase(pos < s->length() ? s->at(pos) : 0);
  char c = (pos < s->length() ? s->at(pos) : 0);
  if(!p) p = new TSTNode(c);
  if(c < p->val)
    p->left = insert(p->left,s,pos);
  else if(c == p->val){
    p->middle =(c ? insert(p->middle,s,pos+1) : 0);
    if(!c) p->data = new string(*s);
  }
  else
    p->right = insert(p->right,s,pos);
  return p;
}

list_t TST::sort() {
  list_t output;
  sort(root,&output);
  return output;
}
void TST::sort(TSTp p, list_t *output) {
  if (!p) return;
  sort(p->left,output);
  if (p->val)
    sort(p->middle,output);
  else
    output->push_back(*p->data);
  sort(p->right,output);
}

void TST::cleanup(TSTp p){
  if(!p) return;
  cleanup(p->left);
  if (p->val) cleanup(p->middle);
  cleanup(p->right);
  delete p;
}

char TST::downcase(char c) {return (c>='A' && c<='Z') ? c-('Z'-'z') : c;}

TSTNode::TSTNode(char val) {this->val = val;left=middle=right=0; data=0;}
TSTNode::~TSTNode() {if(!data) delete data;}
