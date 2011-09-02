#ifndef VIEW_H_
#define VIEW_H_

#include "common.h"
#include <vector>
#include <string>
#include "vt100.h"
#include "matcher.h"
#include "plugins.h"

using namespace std;

class View : public VT100 {
public:
  View();
  ~View();
  void setPrompt(string);
  void refreshLine(string,list_t,list_t);

private:
  void init();
  string formatList(list_t,const string,const string,const string);
  string formatList(list_t,const string,const string,const string,const unsigned int);
  list_t chopList(list_t,const string);
  string prompt;
};


#endif
