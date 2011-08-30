#include "matcher.h"

Matcher::Matcher() {
  init();
}

void Matcher::setDictionary(vector<string> dictionary) {
  this->dictionary = dictionary;
  init();
  history.push(matches(query));
}

void Matcher::init(){
  query="";
}

vector<string> Matcher::getMatches() {
  return history.top();
}

// TODO: /much/ better matcher, at least one that prioritizes length
vector<string> Matcher::matches(string to_match) {
  vector<string> output;
  for(unsigned int i=0;i<dictionary.size();i++)
    if(dictionary.at(i).substr(0,to_match.length()).compare(to_match) == 0)
      output.push_back(dictionary.at(i));
  if(output.size() < 1)
    output.push_back("");
  return output;
}

void Matcher::addChar(char c) {
  bool valid_char=false;
  unsigned int len=query.length()+1;
  // loop through all possible dictionary strings
  for(unsigned int i=0;i<dictionary.size();i++)
    // if cnandidate is long enough and
    if(dictionary.at(i).length() >= len && 
       // is a valid substring of a possible dictionary string
       dictionary.at(i).substr(0,len).compare(query+c) == 0)
      valid_char=true;
  // add char to query
  if(valid_char)
    query += c;

  history.push(matches(query));
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
  vector<string> current_matches = history.top();
  history.pop();
  current_matches.push_back(current_matches.front());
  current_matches.erase(current_matches.begin());
  history.push(current_matches);
}

void Matcher::rotateBackward() {
  vector<string> current_matches = history.top();
  history.pop();
  current_matches.insert(current_matches.begin(),current_matches.back());
  current_matches.pop_back();
  history.push(current_matches);
}
