#include "tokens.h"

Tokens::Tokens() {}
Tokens::~Tokens() {}

list_t Tokens::getTokens() {return tokens;}
void Tokens::push(string token) {tokens.push_back(token);}
void Tokens::pop() {if(tokens.size() > 0) tokens.pop_back();}
