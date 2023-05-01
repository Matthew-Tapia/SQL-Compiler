#ifndef STRING_TOKEN_H
#define STRING_TOKEN_H

#include <iostream>
#include <string>

#include "constants.h"

using namespace std;

class SToken {

public:
  // Constructors
  SToken() {
    _token = "";
    _type = TOKEN_UNKNOWN;
  }

  SToken(string str, STRING_TOKEN_TYPES type) {
    _token = str;
    _type = type;
  }

  // return the type of the token
  STRING_TOKEN_TYPES type() const { return _type; }

  // return the token string
  string token_str() const { return _token; }

  // return the type of the token as a string
  string token_type() const {
    switch (_type) {
    case TOKEN_NUMBER:
      return "NUMBER";
    case TOKEN_ALPHA:
      return "ALPHA";

    case TOKEN_SPACE:
      return "SPACE";

    case TOKEN_OPERATOR:
      return "OPERATOR";

    case TOKEN_COMMA:
      return "COMMA";

    case TOKEN_PARENTH:
      return "PARENTH";

    case TOKEN_STAR:
      return "STAR";

    case TOKEN_STRING:
      return "STRING";

    case TOKEN_UNKNOWN:
      return "UNKNOWN";

    default:
      return "";
    }

    return "";
  }

  // overload the << operator
  friend ostream &operator<<(ostream &outs, const SToken &t) {
    outs << "|" << t.token_str() << '|';
    return outs;
  }

private:
  // the token string
  string _token;
  // the type of the token
  STRING_TOKEN_TYPES _type;
};

#endif // STRING_TOKEN_H