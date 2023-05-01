#ifndef TOKEN_H
#define TOKEN_H
#include "constants.h"
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

class Token {
public:
  Token() {}
  friend ostream &operator<<(ostream &outs, const Token &token) {
    token.Print(outs);
    return outs;
  }
  virtual void Print(ostream &outs = cout) const {
    outs << "TOKEN Print: DON't ^%#$# call me again!" << endl;
  }
  virtual std::string as_string() { return ""; }
  virtual TOKEN_TYPES TypeOf() { return UKNOWN; }
  virtual int get_precedence() { return -1; }
  virtual bool rightAssociative() { return false; }
  virtual vector<long> get_list() { return vector<long>(); }
};

#endif