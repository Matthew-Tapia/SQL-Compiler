#ifndef RELATIONAL_H
#define RELATIONAL_H

#include "constants.h"
#include "token.h"
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

class Relational : public Token {
public:
  Relational() { _str = ""; }
  Relational(const std::string &str) { _str = str; }
  void Print(ostream &outs = cout) const { std::cout << _str; }

  TOKEN_TYPES TypeOf() { return RELATIONAL; }
  string as_string() { return _str; }
  int get_precedence() { return 4; }
  bool rightAssociative() { return false; }

private:
  string _str;
};

#endif