#ifndef VIEW_H_
#define VIEW_H_

#include <vector>
#include <string>
#include "vt100.h"
#include "matcher.h"
#include "verbs.h"

using namespace std;

class View : public VT100 {
public:
  View();
  ~View();
  void setPrompt(string);
  void refreshLine(string,vector<string>,vector<string>);

private:
  void init();
  string formatList(vector<string>,string,string,string);
  string formatList(vector<string>,string,string,string,unsigned int);
  vector<string> chopList(vector<string>,unsigned int);
  string prompt;
};


#endif
