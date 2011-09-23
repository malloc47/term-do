#ifndef HISTORY_H_
#define HISTORY_H_

#include "common.h"
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>

class History {
public:
  History(string);
  ~History() {}
  list_t getSorted(list_t&);

private:
  map<string,int> freq;
  freq_list_t getFrequency(list_t&);
  // http://stackoverflow.com/questions/289347/using-strtok-with-a-string-argument-instead-of-char
  void split(const string& s, const string& delim, vector<string>& parts) {
    size_t start, end = 0;
    while (end < s.size()) {
      start = end;
      while (start < s.size() && (delim.find(s[start]) != string::npos)) {
	start++;  // skip initial whitespace
      }
      end = start;
      while (end < s.size() && (delim.find(s[end]) == string::npos)) {
	end++; // skip to end of word
      }
      if (end-start != 0) {  // just ignore zero-length strings.
	parts.push_back(string(s, start, end-start));
      }
    }
  }
};

#endif
