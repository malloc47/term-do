#include "history.h"

History::History(string filename) {
  ifstream file(filename.c_str());
  if(!file.is_open()) return;
  string line;
  string token;
  while(!file.eof()) {
    list_t split_line;
    getline(file,line);
    split(line," \\:;/'\"|*`.<>\n",split_line);
    FOR_l(i,split_line) {
      string token = split_line[i];
      transform(token.begin(),token.end(),token.begin(),::tolower);
      if(!token.compare("-") || !token.compare("--")) continue;
      if(freq.count(token))
	freq[token]++;
      else
	freq.insert(pair<string,int>(token,1));
    }
  }
}

list_t History::getSorted(list_t& strings) {
  list_t output;
  output.reserve(strings.size()+1);
  freq_list_t freqs = getFrequency(strings);
  sort(freqs.begin(),freqs.end(),greater< pair<int,string> >());
  FOR_l(i,freqs)
    output.push_back(freqs[i].second);
  return output;
}

freq_list_t History::getFrequency(list_t& strings) {
  freq_list_t output;
  output.reserve(strings.size()+1);
  FOR_l(i,strings) {
    if(freq.count(strings[i]))
      output.push_back(pair<int,string>(freq[strings[i]],strings[i]));
    else
      output.push_back(pair<int,string>(0,strings[i]));
  }
  return output;
}
