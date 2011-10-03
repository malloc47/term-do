#include "term-do.h"

TermDo::TermDo() {
  plugins = new Plugins();
  tokens = new Tokens();
  plugins->populate(tokens->getSearcher());
  query = new Query(tokens->getSearcher());
}

TermDo::TermDo(list_t plugin_list) {
  if(plugin_list.empty())
    plugins = new Plugins();
  else
    plugins = new Plugins(plugin_list);
  tokens = new Tokens();
  plugins->populate(tokens->getSearcher());
  query = new Query(tokens->getSearcher());
}

TermDo::~TermDo() {
  delete plugins;
  delete query;
  // delete this *last*
  delete tokens;
}

void TermDo::addToken(string token) {
  Searcher *s = tokens->push(token);
  plugins->update(tokens->getTokens());
  plugins->populate(s);
  query->reset(s);
}

// Removing is cheap thanks to the stack cache
void TermDo::removeToken() {
  if(tokens->getTokens().empty()) return;
  Searcher *s = tokens->pop();
  query->reset(s);
  plugins->update(tokens->getTokens());
}

void TermDo::addChar(char c) {
  query->addChar(c);
  if(c=='/' && query->exactMatch()) commitToken();
}

void TermDo::removeChar() {
  // if query out of characters to backspace
  if(!query->removeChar())
    removeToken();
}

bool TermDo::commitFinalToken() {
  if(query->getMatches().size()>=1 && 
     (tokens->getTokens().size() < 2 || !query->getQuery().empty())) // || query->exactMatch())
    commitValidToken();
  else
    commitToken();
  return (!query->getQuery().empty() || tokens->getTokens().size() > 0);
}

bool TermDo::commitValidToken() {
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

bool TermDo::commitToken() {
  if(query->getQuery().empty())
    return false;
  addToken(query->getQuery());
  return true;
}

void TermDo::reset() {
  tokens->reset();
  query->reset(tokens->getSearcher());
  plugins->update(tokens->getTokens());
}

void TermDo::setCWD(string& cwd) {plugins->setCWD(cwd);}
string& TermDo::getQuery() {return query->getQuery();}
list_t& TermDo::getMatches() {return query->getMatches();}
list_t& TermDo::getTokens() {return tokens->getTokens();}
string TermDo::getCommand() {return plugins->getCommand();}
void TermDo::rotateForward() {query->rotateForward();}
void TermDo::rotateBackward() {query->rotateBackward();}

void TermDo::fullList() {
  Searcher *s = tokens->getSearcher();
  delete s;
  s = new SEARCHER_t();
  plugins->populateAll(s);
  query->reset(s);
}
