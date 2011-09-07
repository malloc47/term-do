#include "matcher.h"

Matcher::Matcher() {
  query="";
  searcher = new TSTLine();
}

Matcher::~Matcher() {
  delete searcher;
}
// The output of this can turn into segfault city if you're not careful
// Oh why can't we have the typesafe haven of Haskell's Maybe monad...
list_t Matcher::getMatches() {
  if(history.empty()) {
    list_t candidates = searcher->searchp(query);
    if(!candidates.empty())
      history.push(candidates);
    else {
      return candidates; // An empty vector output
    }
  }
  return history.top();
}

string Matcher::getMatch() {
  if(history.empty()) return query;
  if(history.top().empty()) return query;
  if(!history.top().front().compare("")) return query;
  return history.top().front();
}

bool Matcher::exactMatch() {
  return searcher->search(query);
}

void Matcher::addCharRestricted(char c) {
  if(searcher->containsp(query+c))
    query += c;
  history.push(searcher->searchp(query));
}

void Matcher::addChar(char c) {
  query += c;
  history.push(searcher->searchp(query));
}

void Matcher::insert(string s) {
  searcher->insert(s);
}

bool Matcher::removeChar() {
  if(query.length()>0) {
    query=query.substr(0,query.length()-1);
    history.pop();
    return true;
  }
  return false;
}

string Matcher::getQuery() {return query;}

void Matcher::rotateForward() {
  if(history.top().size()<2) return;
  list_t current_matches = history.top();
  history.pop();
  current_matches.push_back(current_matches.front());
  current_matches.erase(current_matches.begin());
  history.push(current_matches);
}

void Matcher::rotateBackward() {
  if(history.top().size()<2) return;
  list_t current_matches = history.top();
  history.pop();
  current_matches.insert(current_matches.begin(),current_matches.back());
  current_matches.pop_back();
  history.push(current_matches);
}
