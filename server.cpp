#include "server.h"

Server::Server() {
  plugins = new Plugins();
  initMatcher();
}

Server::Server(list_t plugin_list) {
  if(plugin_list.empty())
    plugins = new Plugins();
  else
    plugins = new Plugins(plugin_list);
  initMatcher();
}

Server::~Server() {
  delete plugins;
  cleanupMatcher();
}

void Server::initMatcher() {
  matcher = new Matcher();
  plugins->getDictionary(matcher);
}

void Server::cleanupMatcher() {
  delete matcher;
}

void Server::addChar(char c) {
  matcher->addChar(c);
  if(c=='/' && matcher->exactMatch()) commitToken();
}

void Server::removeChar() {
  // if matcher out of characters to backspace
  if(!matcher->removeChar()) {
    // remove token
    plugins->pop();
    cleanupMatcher();
    initMatcher();
  }
}

bool Server::commitFinalToken() {
  if(matcher->getMatches().size()<=1 || matcher->exactMatch())
    commitValidToken();
  return (!matcher->getQuery().empty() || plugins->getTokens().size() > 0);
}

bool Server::commitValidToken() {
  string match = matcher->getMatch();
  if(match.empty())
    return false;
  plugins->push(match);
  cleanupMatcher();
  initMatcher();
  return true;
}

bool Server::commitToken() {
  if(matcher->getQuery().empty())
    return false;
  plugins->push(matcher->getQuery());
  cleanupMatcher();
  initMatcher();
  return true;
}

string Server::getQuery() {matcher->getQuery();}
list_t Server::getMatches() {matcher->getMatches();}
list_t Server::getTokens() {plugins->getTokens();}

void Server::rotateForward() {matcher->rotateForward();}

void Server::rotateBackward() {matcher->rotateBackward();}
