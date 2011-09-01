#include "tst.h"

// http://stackoverflow.com/questions/2146792/how-do-you-generate-random-strings-in-
// only using a fixed version, obviously...
string randomStrGen(int length) {
    static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    string result;
    result.resize(length);
    for (int i = 0; i < length; i++)
        result[i] = charset[rand() % charset.length()];
    return result;
}

TST::TST() {root=0; srand(time(NULL)); /* <- debug */}
TST::~TST() {cleanup(root);}

bool TST::search(string s) {
  if(s.empty()) return false;
  TSTp p = root;
  unsigned int pos = 0;
  while(p) {
    char c = (pos < s.length() ? s[pos] : 0);
    // printf("%c == %c\n",c,p->val);
    if(c < p->val)
      p=p->left;
    else if(c == p->val) {
      // if(s.at(pos+1) == 0)
      if(p->data) return true;
      p=p->middle;
      pos++;
    }
    else
      p=p->right;
  }
  return false;
  //return search(root,&s,0);
}
bool TST::search(TSTp p, string *s, unsigned int pos) {
  if(!p || s->empty()) return false;
  char c = s->at(pos);
  if(c < p->val)
    return search(p->left,s,pos);
  else if(c > p->val)
    return search(p->right,s,pos);
  else
    if(pos==s->size()-1 && p->data)
      return true;
  return (pos==s->size()-1 ? 0 :search(p->middle,s,pos+1));
}

vector<string> TST::hammingSearch(string s,int d) {
  vector<string> output;
  hammingSearch(root,&s,0,d,&output);
  return output;
}

void TST::hammingSearch(TSTp p, string *s, unsigned int pos, int d, vector<string> *output) {
  if(!p || d<0) return;
  char c = (pos < s->length() ? s->at(pos) : 0);
  if(d>0 || c < p->val)
    hammingSearch(p->left,s,pos,d,output);
  printf("%c==%c\n",c,p->val);
  if(p->val == 0) {
    if( s->length()-pos <= d)
      output->push_back(*p->data);
  }
  else
    hammingSearch(p->middle,s,c ? pos+1:pos, (c==p->val) ? d:d-1,output);
  if(d>0 || c > p->val)
    hammingSearch(p->right,s,pos,d,output);
}

void TST::insert(string s) {root = insert(root,&s,0);}
TSTp TST::insert(TSTp p, string *s, unsigned int pos) {
  if(s->empty()) return p;
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

void TST::traverse() {traverse(root);}
void TST::traverse(TSTp p) {
  if (!p) return;
    traverse(p->left);
    if (p->val)
        traverse(p->middle);
    else
      printf("%s\n", p->data->c_str());
    traverse(p->right);
}

void TST::cleanup(TSTp p){
  if(!p) return;
  cleanup(p->left);
  if (p->val) cleanup(p->middle);
  cleanup(p->right);
  delete p;
}

TSTNode::TSTNode(char val) {this->val = val;left=middle=right=0; data=0;}
TSTNode::~TSTNode() {if(!data) delete data;}

int main(int argc, char *argv[]) {
  TST tst;
  // tst.insert("Test");
  // tst.insert("Tes7");
  // tst.insert("0101");
  // tst.insert("0111");
  tst.insert("best");  
  tst.insert("Jesus");
  tst.insert("beste");
  tst.insert("fest");
  tst.insert("tent");  
  tst.insert("test");
  tst.insert("testy");  
  tst.insert("text");
  tst.insert("beet");
  // tst.insert("Really Really Long, very long string... 47");
  /*  for(unsigned int i=0;i<100000;i++) {
    string test = randomStrGen(10);
    tst.insert(test);
    if(!tst.search(test)) printf("AAAAHHHH!!!!");
    } 
  */
  // tst.traverse();
  printf("%d\n",tst.search("test"));
  printf("%d\n",tst.search("beste"));
  printf("%d\n",tst.search("tes"));
  printf("%d\n",tst.search("beet"));
  printf("%d\n",tst.search("t"));
  printf("%d\n",tst.search("testy"));

  // printf("%d\n",tst.search("Test"));
  // printf("%d\n",tst.search("T"));
  // printf("%d\n",tst.search("Tes"));
  // printf("%d\n",tst.search("Tes7"));
  // printf("%d\n",tst.search("Test8"));
  // printf("%d\n",tst.search("Tes71"));

  vector<string> output = tst.hammingSearch("test",2);
  printf("\n%d\n",output.size());
  for(unsigned int i=0;i<output.size();i++)
      printf("%s\n",output[i].c_str());

}
