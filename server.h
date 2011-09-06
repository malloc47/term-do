#ifndef SERVER_H_
#define SERVER_H_

#include "common.h"

#include "matcher.h"
#include "plugins.h"
#include <string>
#include <vector>

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
  list_t getMatches();
  list_t getTokens();

private:
  void initMatcher();
  void cleanupMatcher();
  Matcher *matcher;
  Plugins *plugins;
};

#endif
