#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include <cassert>
#include <iostream>

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

class ShuntingYard {
public:
  // CTOR
  ShuntingYard() {}

  ShuntingYard(const Queue<Token *> &input_q) { queue = input_q; }

  // save input_q to member variable queue
  void infix(const Queue<Token *> &input_q) { queue = input_q; }

  // generate postfix queue from infix queue
  Queue<Token *> postfix() { return shunting_yard(); }

  Queue<Token *> postfix(const Queue<Token *> &input_q) {
    queue = input_q;

    return shunting_yard();
  }

  // called by postfix()
  Queue<Token *> shunting_yard() {
    Queue<Token *> output_queue;
    Stack<Token *> operator_stack;

    for (Token *t : queue) {

      if (t->TypeOf() == STR) {
        output_queue.push(t);
      }

      if (t->TypeOf() == LOGICAL || t->TypeOf() == RELATIONAL) {
        Token *o1 = t;

        while (!operator_stack.empty()) {
          Token *o2 = operator_stack.top();

          if (o2->TypeOf() != L_PAREN &&
              ((!o1->rightAssociative() &&
                o1->get_precedence() <= o2->get_precedence()) ||
               (o1->rightAssociative() &&
                o1->get_precedence() < o2->get_precedence()))) {
            operator_stack.pop();
            output_queue.push(o2);

            continue;
          }
          break;
        }

        operator_stack.push(o1);
      }

      if (t->TypeOf() == L_PAREN) {
        operator_stack.push(t);
      }

      if (t->TypeOf() == R_PAREN) {
        while (!operator_stack.empty() &&
               operator_stack.top()->TypeOf() != L_PAREN) {

          output_queue.push(operator_stack.top());
          operator_stack.pop();
        }

        operator_stack.pop();
      }
    }

    while (!operator_stack.empty()) {
      output_queue.push(operator_stack.top());
      operator_stack.pop();
    }

    return output_queue;
  }

private:
  Queue<Token *> queue;
};

#endif // SHUNTING_YARD_H