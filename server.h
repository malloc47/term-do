#ifndef SERVER_H_
#define SERVER_H_

#include "common.h"
#include "query.h"
#include "plugins.h"
#include "tokens.h"
#include "cache.h"
#include <string>
#include <vector>
#include <stack>

class Server {
public:
  Server();
  Server(list_t);
  ~Server();
  void addChar(char);
  void removeChar();
  bool commitFinalToken();
  bool commitValidToken();
  bool commitToken();
  void rotateForward();
  void rotateBackward();
  string getQuery();
  string getCommand();
  list_t getMatches();
  list_t getTokens();

private:
  void addToken(string);
  void removeToken();
  Tokens tokens;
  Cache cache;
  Query *query;
  Plugins *plugins;
};

#endif
