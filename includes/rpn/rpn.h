#ifndef RPN_H
#define RPN_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "../map/map.h"
#include "../map/mmap.h"

#include "../linked_list/queue/MyQueue.h"
#include "../linked_list/stack/MyStack.h"
#include "../token/leftparen.h"
#include "../token/logical.h"
#include "../token/relational.h"
#include "../token/rightparen.h"
#include "../token/token.h"
#include "../token/tokenstr.h"
#include "../token/universal.h"

using namespace std;

class RPN {
public:
  // CTOR
  RPN() {}

  RPN(const Queue<Token *> &input_q) { queue = input_q; }

  // save input_q to member variable queue
  void set_input(const Queue<Token *> &input_q) { queue = input_q; }

  // return result from member variable queue
  std::vector<long> operator()(std::vector<std::string> fields,
                               vector<MMap<std::string, long>> columns) {

    return rpn(fields, columns);
  }

  // called by operator ()
  std::vector<long> rpn(std::vector<std::string> fields,
                        vector<MMap<std::string, long>> columns) {

    Token *a = new Token();
    Token *b = new Token();

    Stack<Token *> stack;
    for (Token *t : queue) {
      if (isString(t)) {
        stack.push(t);
      } else if (isRelational(t)) {

        b = stack.top();
        stack.pop();

        a = stack.top();
        stack.pop();

        int i = 0;
        for (; i < fields.size(); ++i) {
          if (fields[i] == a->as_string()) {
            break;
          }
        }

        std::vector<long> eval;

        if (fields[i] == a->as_string()) {
          eval = evaluateRelational(columns[i], b->as_string(), t->as_string());
        }

        Universal *temp = new Universal(eval);

        stack.push(temp);

      } else if (isLogical(t)) {
        a = stack.top();
        stack.pop();

        b = stack.top();
        stack.pop();

        std::vector<long> eval;

        eval = evaluateLogical(a->get_list(), b->get_list(), t->as_string());

        Universal *temp = new Universal(eval);

        stack.push(temp);
      }
    }

    return stack.top()->get_list();
  }

  bool isRelational(Token *t) { return t->TypeOf() == RELATIONAL; }

  bool isLogical(Token *t) { return t->TypeOf() == LOGICAL; }

  bool isString(Token *t) { return t->TypeOf() == STR; }

  std::vector<long> evaluateRelational(MMap<std::string, long> column,
                                       std::string con, std::string op) {
    if (op == "<") {
      return evaluateLessThan(column, con);
    } else if (op == ">") {
      return evaluateGreaterThan(column, con);
    } else if (op == "<=") {
      return evaluateLessThanEqualTo(column, con);
    } else if (op == ">=") {
      return evaluateGreaterThanEqualTo(column, con);
    } else if (op == "=") {
      return evaluateEqualTo(column, con);
    } else if (op == "!=") {
      return evaluateNotEqualTo(column, con);
    } else {
      return std::vector<long>();
    }

    return std::vector<long>();
  }

  std::vector<long> evaluateLogical(std::vector<long> lhs,
                                    std::vector<long> rhs, std::string op) {
    if (op == "and") {
      return evaluateAnd(lhs, rhs);
    } else if (op == "or") {
      return evaluateOr(lhs, rhs);
    } else if (op == "not") {
      // IMPLEMENT NOT
    } else {
      return std::vector<long>();
    }

    return std::vector<long>();
  }

  std::vector<long> evaluateLessThan(MMap<std::string, long> column,
                                     std::string con) {

    std::vector<long> result;

    MMap<std::string, long>::Iterator walker = column.begin();

    while (walker != column.end()) {
      if ((*(walker)).key < con) {
        append(result, (*(walker)).value_list);
      }

      ++walker;
    }

    return result;
  }

  std::vector<long> evaluateLessThanEqualTo(MMap<std::string, long> column,
                                            std::string con) {

    std::vector<long> result;

    MMap<std::string, long>::Iterator walker = column.begin();

    while (walker != column.end()) {
      if ((*(walker)).key <= con) {
        append(result, (*(walker)).value_list);
      }

      ++walker;
    }

    return result;
  }

  std::vector<long> evaluateGreaterThan(MMap<std::string, long> column,
                                        std::string con) {

    std::vector<long> result;

    MMap<std::string, long>::Iterator walker = column.begin();

    while (walker != column.end()) {
      if ((*(walker)).key > con) {
        append(result, (*(walker)).value_list);
      }

      ++walker;
    }

    return result;
  }

  std::vector<long> evaluateGreaterThanEqualTo(MMap<std::string, long> column,
                                               std::string con) {

    std::vector<long> result;

    MMap<std::string, long>::Iterator walker = column.begin();

    while (walker != column.end()) {
      if ((*(walker)).key >= con) {
        append(result, (*(walker)).value_list);
      }

      ++walker;
    }

    return result;
  }

  std::vector<long> evaluateEqualTo(MMap<std::string, long> column,
                                    std::string con) {

    std::vector<long> result;

    MMap<std::string, long>::Iterator walker = column.begin();

    while (walker != column.end()) {
      if ((*(walker)).key == con) {
        append(result, (*(walker)).value_list);
      }

      ++walker;
    }

    return result;
  }

  std::vector<long> evaluateNotEqualTo(MMap<std::string, long> column,
                                       std::string con) {

    std::vector<long> result;

    MMap<std::string, long>::Iterator walker = column.begin();

    while (walker != column.end()) {
      if ((*(walker)).key != con) {
        append(result, (*(walker)).value_list);
      }

      ++walker;
    }

    return result;
  }

  std::vector<long> evaluateAnd(std::vector<long> lhs, std::vector<long> rhs) {

    std::sort(lhs.begin(), lhs.end());
    std::sort(rhs.begin(), rhs.end());

    std::vector<long> result;

    std::set_intersection(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                          back_inserter(result));

    return result;
  }

  std::vector<long> evaluateOr(std::vector<long> lhs, std::vector<long> rhs) {

    std::sort(lhs.begin(), lhs.end());
    std::sort(rhs.begin(), rhs.end());

    std::vector<long> result;

    std::set_union(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                   back_inserter(result));

    return result;
  }

  void append(std::vector<long> &lhs, const std::vector<long> rhs) {
    for (int i = 0; i < rhs.size(); ++i) {
      lhs.push_back(rhs[i]);
    }
  }

  // std::vector<long> intersection(std::vector<long> lhs, std::vector<long>
  // rhs){
  //   std::vector
  // }

private:
  Queue<Token *> queue;
};

#endif // RPN_H