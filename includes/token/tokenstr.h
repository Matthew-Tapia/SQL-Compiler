#ifndef TOKENSTR_H
#define TOKENSTR_H

#include "constants.h"
#include "token.h"
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

class TokenStr : public Token {
public:
  TokenStr() { _str = ""; }
  TokenStr(string str) { _str = str; }
  void Print(ostream &outs = cout) const { std::cout << _str; }

  TOKEN_TYPES TypeOf() { return STR; }
  string as_string() { return _str; }
  int get_precedence() { return 0; }
  bool rightAssociative() { return false; }

private:
  string _str;
};

#endif
