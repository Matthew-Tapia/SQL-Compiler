#ifndef UNIVERSAL_H
#define UNIVERSAL_H

#include "constants.h"
#include "token.h"
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

class Universal : public Token {
public:
  Universal() {}
  Universal(const vector<long> &record_nums) { _record_nums = record_nums; }
  void Print(ostream &outs = cout) const {
    for (int i = 0; i < _record_nums.size(); ++i) {
      outs << _record_nums[i] << " ";
    }
  }

  TOKEN_TYPES TypeOf() { return UNIVERSAL; }
  string as_string() {
    std::string result = "";
    for (int i = 0; i < _record_nums.size(); ++i) {
      result += (std::to_string(_record_nums[i]) + " ");
    }

    return result;
  }
  int get_precedence() { return -1; }
  bool rightAssociative() { return false; }

  vector<long> get_list() { return _record_nums; }

private:
  vector<long> _record_nums;
};

#endif