#ifndef SQL_TOKEN_H
#define SQL_TOKEN_H

#include <iostream>
#include <string>

#include "constants.h"

using namespace std;

class SQLToken {

public:
  // Constructors
  SQLToken() {
    _token = "";
    _type = SQL_UNKNOWN;
  }

  SQLToken(string str, SQL_KEYWORDS type) {
    _token = str;
    _type = type;
  }

  // return the type of the token
  SQL_KEYWORDS type() const { return _type; }

  // return the token string
  string token_str() const { return _token; }

  // return the type of the token as a string
  string token_type() const {
    if (_type == SQL_UNKNOWN) {
      return "UNKNOWN";
    }

    return (const char *[]){
        "CREATE", "TABLE",    "FIELDS",       "INSERT",        "INTO",
        "VALUES", "SELECT",   "FROM",         "WHERE",         "AND",
        "OR",     "NOT",      "WORD",         "NUMBER",        "STRING",
        "COMMA",  "OPERATOR", "LEFT_PARENTH", "RIGHT_PARENTH", "SPACE",
        "STAR"}[_type];
  }

  // overload the << operator
  friend ostream &operator<<(ostream &outs, const SQLToken &t) {
    outs << "|" << t.token_str() << '|';
    return outs;
  }

private:
  // the token string
  string _token;
  // the type of the token
  SQL_KEYWORDS _type;
};

#endif