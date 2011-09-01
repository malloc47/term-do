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
    char c = downcase(pos < s.length() ? s[pos] : 0);
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

bool TST::rsearch(string s) { return rsearch(root,&s,0);}
bool TST::rsearch(TSTp p,string *s, unsigned int pos) {
  if(!p || s->empty()) return false;
  char c = downcase(pos < s->length() ? s->at(pos) : 0);
  if(c < p->val)
    return rsearch(p->left,s,pos);
  else if(c > p->val)
    return rsearch(p->right,s,pos);
  else {
    if(c==0) return true;
    return rsearch(p->middle,s,pos+1);
  }
}

vector<string> TST::prefixSearch(string s) { 
  vector<string> output;
  prefixSearch(root,&s,0,&output);
  return output;
}
void TST::prefixSearch(TSTp p,string *s, unsigned int pos, vector<string> *output) {
  if(!p || s->empty()) return;
  char c = downcase(pos < s->length() ? s->at(pos) : 0);
  if(c < p->val)
    prefixSearch(p->left,s,pos,output);
  else if(c > p->val)
    prefixSearch(p->right,s,pos,output);
  else {
    // if(c==0) output->push_back(*p->data);
    if(pos==s->length()-1)
      sort(p->middle,output);
    prefixSearch(p->middle,s,pos+1,output);
  }
}

// vector<string> TST::prefixSearch(string s) {
//   vector<string> output;
//   if(s.empty()) return output;
//   TSTp p = root;
//   unsigned int pos = 0;
//   while(p) {
//     char c = downcase(pos < s.length() ? s[pos] : 0);
//     printf("%c==%c\t%d==%d\n",c,p->val,pos,s.length());
//     if(c < p->val)
//       p=p->left;
//     else if(c == p->val && (pos<s.length()-1)) {
//       p=p->middle;
//       pos++;
//     }
//     else if((pos==s.length()-1) && s[pos+1]==0) {
//       printf("Here\n");
//       if(p->middle) {
// 	sort(p->middle,&output);
// 	return output;
//       }
//     }
//     else
//       p=p->right;
//   }
//   return output;
// }


vector<string> TST::hammingSearch(string s,int d) {
  vector<string> output;
  hammingSearch(root,&s,0,d,&output);
  return output;
}
void TST::hammingSearch(TSTp p, string *s, unsigned int pos, int d, vector<string> *output) {
  if(!p || d<0) return;
  char c = downcase(pos < s->length() ? s->at(pos) : 0);
  if(d>0 || c < p->val)
    hammingSearch(p->left,s,pos,d,output);
  if(p->val == 0) {
    // if( s->length()-pos <= d)
    if(pos >= s->length()) // no shorter words
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
  char c = downcase(pos < s->length() ? s->at(pos) : 0);
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

vector<string> TST::sort() {
  vector<string> output;
  sort(root,&output);
  return output;
}
void TST::sort(TSTp p, vector<string> *output) {
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

char TST::downcase(char c) {return c;} //(c>='A' && c<='Z') ? c-('Z'-'z') : c;}

TSTNode::TSTNode(char val) {this->val = val;left=middle=right=0; data=0;}
TSTNode::~TSTNode() {if(!data) delete data;}

int main(int argc, char *argv[]) {
  TST tst;
  // tst.insert("Test");
  // tst.insert("Tes7");
  // tst.insert("0101");
  // tst.insert("0111");
  // tst.insert("best");  
  // tst.insert("beste");
  // tst.insert("fest");
  // tst.insert("tent");  
  // tst.insert("test");
  // tst.insert("testy");  
  // tst.insert("text");
  // tst.insert("beet");
  // tst.insert("te");
  // tst.insert("te#");

  tst.insert("Really Really Long, very long string... 47");
  for(unsigned int i=0;i<500000;i++) {
    string test = randomStrGen(5);
    tst.insert(test);
    if(!tst.search(test)) printf("AAAAHHHH!!!!");
  } 
  // printf("%d\n",tst.search("test"));
  // printf("%d\n",tst.search("beste"));
  // printf("%d\n",tst.search("tes"));
  // printf("%d\n",tst.search("beet"));
  // printf("%d\n",tst.search("t"));

  // printf("%d\n",tst.search("Test"));
  // printf("%d\n",tst.search("T"));
  // printf("%d\n",tst.search("Tes"));
  // printf("%d\n",tst.search("Tes7"));
  // printf("%d\n",tst.search("Test8"));
  // printf("%d\n",tst.search("Tes71"));

  // vector<string> output = tst.hammingSearch("test",2);
  // printf("\n%d\n",output.size());
  // for(unsigned int i=0;i<output.size();i++)
  //     printf("%s\n",output[i].c_str());

  // string line;
  // ifstream myfile ("/home/malloc47/tmp/downloads/dictwords.txt");
  // if (myfile.is_open()) {
  //   while ( myfile.good() ) {
  //     getline (myfile,line);
  //     tst.insert(line);
  //   }
  //   myfile.close();
  // }

  // vector<string> output = tst.hammingSearch("abbre",1);
  // vector<string> output = tst.sort();
  vector<string> output = tst.prefixSearch("474");
  // vector<string> output = tst.hammingSearch("ase",1);
  printf("\n%d\n\n",output.size());
  for(unsigned int i=0;i<output.size();i++)
    if(output.at(i).substr(0,3).compare("474") != 0){
      printf("%s\t%s\t%d\n",output[i].c_str(),output.at(i).substr(0,2).c_str(),output.at(i).substr(0,2).compare("16"));
    }


}
