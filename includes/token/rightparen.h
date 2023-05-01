#ifndef RIGHTPAREN_H
#define RIGHTPAREN_H

#include "constants.h"
#include "token.h"
#include <iomanip>
#include <iostream>

using namespace std;

class RightParen : public Token {
public:
  RightParen() { _str = ')'; }
  void Print(ostream &outs = cout) const { outs << _str; }

  TOKEN_TYPES TypeOf() { return R_PAREN; }
  string as_string() { return _str; }
  int get_precedence() { return 1; }
  bool rightAssociative() { return false; }

private:
  string _str;
};

#endif