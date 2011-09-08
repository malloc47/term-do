#include "tokens.h"

Tokens::Tokens() {}

Tokens::~Tokens() {}

list_t Tokens::getTokens() {return tokens;}
void Tokens::push(string token) {tokens.push_back(token);}
void Tokens::pop() {if(tokens.size() > 0) tokens.pop_back();}

// bool Tokens::commitToken(string token) {
//   if(token.empty()) return false;
//   addToken(token);
//   return true;
// }

// bool Tokens::commitValidToken(string token) {
//   if(token.empty()) return false;
//   if(token.find(" ")!=string::npos) {
//     stringstream ss(token);
//     string item;
//     while(getline(ss,item,' '))
//       if(!item.empty() && item.compare(" "))
// 	addToken(item);
//   }
//   else
//     addToken(token);
//   return true;
// }

// bool Tokens::commitFinalToken(string token) {
//   // Add exactMatch() logic in elsewhere
//   if(token.size()<=1) //  || query->exactMatch()
//     commitValidToken(token);
//   return (!token.empty() || tokens.size() > 0);
// }
