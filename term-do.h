#ifndef TERMDO_H_
#define TERMDO_H_

#include "common.h"
#include "query.h"
#include "plugins.h"
#include "tokens.h"
#include "cache.hpp"
#include <string>
#include <vector>
#include <stack>

class TermDo {
public:
  TermDo();
  TermDo(list_t);
  ~TermDo();
  void addChar(char);
  void removeChar();
  bool commitFinalToken();
  bool commitValidToken();
  bool commitToken();
  void rotateForward();
  void rotateBackward();
  void fullList();
  void setCWD(string&);
  string& getQuery();
  string getCommand();
  list_t& getMatches();
  list_t& getTokens();
  void reset();

private:
  void addToken(string);
  void removeToken();
  Tokens *tokens;
  Query *query;
  Plugins *plugins;
};

#endif
