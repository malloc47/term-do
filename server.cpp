#include "server.h"

Server::Server() {
  plugins = new Plugins();
  plugins->populate(tokens->getSearcher());
  query = new Query(tokens->getSearcher());
}

Server::Server(list_t plugin_list) {
  if(plugin_list.empty())
    plugins = new Plugins();
  else
    plugins = new Plugins(plugin_list);
  tokens = new Tokens();
  plugins->populate(tokens->getSearcher());
  query = new Query(tokens->getSearcher());
}

Server::~Server() {
  delete plugins;
  delete query;
  // delete this *last*
  delete tokens;
}

void Server::addToken(string token) {
  Searcher *s = tokens->push(token);
  plugins->update(tokens->getTokens());
  plugins->populate(s);
  query->reset(s);
}

// Removing is cheap thanks to the stack cache
void Server::removeToken() {
  if(tokens->getTokens().empty()) return;
  Searcher *s = tokens->pop();
  query->reset(s);
  plugins->update(tokens->getTokens());
}

void Server::addChar(char c) {
  query->addChar(c);
  if(c=='/' && query->exactMatch()) commitToken();
}

void Server::removeChar() {
  // if query out of characters to backspace
  if(!query->removeChar())
    removeToken();
}

bool Server::commitFinalToken() {
  if(query->getMatches().size()>=1 && 
     (tokens->getTokens().size() < 2 || !query->getQuery().empty())) // || query->exactMatch())
    commitValidToken();
  else
    commitToken();
  return (!query->getQuery().empty() || tokens->getTokens().size() > 0);
}

bool Server::commitValidToken() {
  string match = query->getMatch();
  if(match.empty())
    return false;
  if(match.find(" ")!=string::npos) {
    stringstream ss(match);
    string item;
    while(getline(ss,item,' '))
      if(!item.empty() && item.compare(" "))
	addToken(item);
  }
  else
    addToken(match);
  return true;
}

bool Server::commitToken() {
  if(query->getQuery().empty())
    return false;
  addToken(query->getQuery());
  return true;
}

string Server::getQuery() {return query->getQuery();}
list_t Server::getMatches() {return query->getMatches();}
list_t Server::getTokens() {return tokens->getTokens();}
string Server::getCommand() {return plugins->getCommand();}
void Server::rotateForward() {query->rotateForward();}
void Server::rotateBackward() {query->rotateBackward();}
void Server::fullList() {
  Searcher *s = tokens->getSearcher();
  delete s;
  s = new SEARCHER_t();
  plugins->populateAll(s);
  query->reset(s);
}
