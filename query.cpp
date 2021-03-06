#include "query.h"

Query::Query(Searcher *s) : sorter(string(getenv("HOME"))+"/.bash_history") {
  query="";
  searcher = s;
}

Query::~Query() {
  // DON'T DELETE searcher HERE
}

list_t& Query::getMatches() {
  if(history.empty()) {
    list_t unsorted_candidates = searcher->searchp(query);
    list_t candidates = sorter.getSorted(unsorted_candidates);
    if(!candidates.empty())
      history.push(candidates);
    else {
      // TODO: something better here
      return empty; // An empty vector output
    }
  }
  return history.top();
}

string Query::getMatch() {
  if(history.empty() || 
     history.top().empty() ||
     !history.top().front().compare("")) 
    return query;
  return history.top().front();
}

bool Query::exactMatch() {
  return searcher->search(query);
}

// void Query::addCharRestricted(char c) {
//   if(searcher->containsp(query+c))
//     query += c;
//   history.push(searcher->searchp(query));
// }

void Query::addChar(char c) {
  query += c;
  list_t unsorted_candidates = searcher->searchp(query);
  list_t candidates = sorter.getSorted(unsorted_candidates);
  history.push(candidates);
}

bool Query::removeChar() {
  if(query.length()>0) {
    query=query.substr(0,query.length()-1);
    history.pop();
    return true;
  }
  return false;
}

string& Query::getQuery() {return query;}

void Query::rotateForward() {
  if(history.top().size()<2) return;
  list_t current_matches = history.top();
  history.pop();
  current_matches.push_back(current_matches.front());
  current_matches.erase(current_matches.begin());
  history.push(current_matches);
}

void Query::rotateBackward() {
  if(history.top().size()<2) return;
  list_t current_matches = history.top();
  history.pop();
  current_matches.insert(current_matches.begin(),current_matches.back());
  current_matches.pop_back();
  history.push(current_matches);
}

void Query::reset(Searcher *s) {
  query=""; 
  while(!history.empty())
    history.pop();
  searcher=s;
}
