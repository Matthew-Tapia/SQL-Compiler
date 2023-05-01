#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

template <typename T> struct node {
public:
  node(const T &item = T(), node *next = nullptr)
      : _item(item), _next(next), _prev(nullptr) {}

  template <typename U>
  friend ostream &operator<<(ostream &outs, const node<U> &printMe);

  T _item;
  node *_next;
  node *_prev;
};

// Definition

template <typename U>
ostream &operator<<(ostream &outs, const node<U> &printMe) {
  outs << "<-[" << printMe._item << "]->";
  return outs;
}

// TODO

#endif // NODE_H