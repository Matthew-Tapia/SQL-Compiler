#ifndef LEFTPAREN_H
#define LEFTPAREN_H

#include "constants.h"
#include "token.h"
#include <iomanip>
#include <iostream>

using namespace std;

class LeftParen : public Token {
public:
  LeftParen() { _str = '('; }
  void Print(ostream &outs = cout) const { outs << _str; }

  TOKEN_TYPES TypeOf() { return L_PAREN; }
  string as_string() { return _str; }
  int get_precedence() { return 0; }
  bool rightAssociative() { return false; }

private:
  string _str;
};

#endif