#include "matcher.h"

Matcher::Matcher() {
  query="";
  // history.push(matches(query));
}

Matcher::~Matcher() {}

list_t Matcher::getMatches() {
  if(history.empty()) history.push(matches(query));
  return history.top();
}

list_t Matcher::matches(string to_match) {
  // list_t output;
  // for(unsigned int i=0;i<dictionary.size();i++)
  //   if(dictionary.at(i).substr(0,to_match.length()).compare(to_match) == 0)
  //     output.push_back(dictionary.at(i));
  // if(output.size() < 1)
  //   output.push_back("");
  // std::sort(output.begin(),output.end());
  // return output;
  list_t matched = searchPrefix(to_match);
  if(history.empty()) history.push(matched);
  return matched;
}

void Matcher::addChar(char c) {
  if(containsPrefix(query+c))
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
