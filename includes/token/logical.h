#ifndef LOGICAL_H
#define LOGICAL_H

#include "constants.h"
#include "token.h"
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

class Logical : public Token {
public:
  Logical() {}
  Logical(const std::string &str) { _str = str; }
  void Print(ostream &outs = cout) const { outs << _str; }
  TOKEN_TYPES TypeOf() { return LOGICAL; }
  string as_string() { return _str; }
  int get_precedence() {
    if (_str == "not") {
      return 3;
    } else if (_str == "and") {
      return 2;
    } else if (_str == "or") {
      return 1;
    } else {
      return -1;
    }
  }

  bool rightAssociative() {
    if (_str == "not") {
      return true;
    } else if (_str == "and") {
      return false;
    } else if (_str == "or") {
      return false;
    } else {
      return false;
    }
  }

private:
  string _str;
};

#endif