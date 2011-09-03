#include "matcher.h"

Matcher::Matcher() {
  query="";
}

Matcher::~Matcher() {}
// The output of this can turn into segfault city if you're not careful
// Oh why can't we have the typesafe haven of Haskell's Maybe monad...
list_t Matcher::getMatches() {
  if(history.empty()) {
    list_t candidates = matches(query);
    if(!candidates.empty())
      history.push(candidates);
    else {
      return candidates; // An empty vector output
    }
  }
  return history.top();
}

string Matcher::getMatch() {
  if(history.empty())
    return query;

  if(history.top().empty())
    return query;

  if(!history.top().front().compare(""))
    return query;

  return history.top().front();
}

bool Matcher::exactMatch() {
  return search(query);
}

list_t Matcher::matches(string to_match) {
  list_t matched_prefix = searchPrefix(to_match);
  // list_t matched_prefix_2 = searchPrefix("-"+to_match);
  // list_t matched_prefix_3 = searchPrefix("--"+to_match);
  // list_t matched_hamming = searchHamming(to_match,2);
  list_t matched = matched_prefix;
  // matched.reserve(matched_prefix.size() + matched_prefix_2.size() + matched_prefix_3.size());
  // matched.insert(matched.end(),matched_prefix.begin(),matched_prefix.end());
  // matched.insert(matched.end(),matched_prefix_2.begin(),matched_prefix_2.end());
  // matched.insert(matched.end(),matched_prefix_3.begin(),matched_prefix_3.end());

  // matched.insert(matched.end(),matched_hamming.begin(),matched_hamming.end());
  // std::sort(matched.begin(),matched.end() );
  // matched.erase(unique(matched.begin(),matched.end()),matched.end());
  return matched;
}

void Matcher::addCharRestricted(char c) {
  if(containsPrefix(query+c))
    query += c;
  history.push(matches(query));
}

void Matcher::addChar(char c) {
  query += c;
  history.push(matches(query));
}

// void Matcher::addChar(char c) {
//   bool valid_char=false;
//   unsigned int len=query.length()+1;
//   // loop through all possible dictionary strings
//   for(unsigned int i=0;i<dictionary.size();i++)
//     // if cnandidate is long enough and
//     if(dictionary.at(i).length() >= len && 
//        // is a valid substring of a possible dictionary string
//        dictionary.at(i).substr(0,len).compare(query+c) == 0)
//       valid_char=true;
//   // add char to query
//   if(valid_char)
//     query += c;

//   history.push(matches(query));
// }

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
