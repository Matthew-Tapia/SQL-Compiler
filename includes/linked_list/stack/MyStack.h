#ifndef MYSTACK_H
#define MYSTACK_H

#include <iomanip>
#include <iostream>

#include "../linked_list_functions/linked_list_functions.h"

using namespace std;

template <typename T>
class Stack {
 public:
  class Iterator {
   public:
    // Give access to list to access _ptr
    friend class Stack;

    // Default CTOR
    Iterator() { _ptr = nullptr; }
    // Point Iterator to where p is pointing to
    Iterator(node<T> *p) { _ptr = p; }

    // Casting operator: true if _ptr not NULL
    // This turned out to be a pain!
    operator bool() { return (_ptr != nullptr); }

    // dereference operator
    T &operator*() { return _ptr->_item; }

    // member access operator
    T *operator->() { return _ptr->_item; }

    // true if _ptr is NULL
    bool is_null() { return _ptr == nullptr; }

    // true if left != right
    friend bool operator!=(const Iterator &left, const Iterator &right) {
      return left._ptr != right._ptr;
    }
    // true if left == right
    friend bool operator==(const Iterator &left, const Iterator &right) {
      return left._ptr == right._ptr;
    }

    // member operator:  ++it; or ++it = new_value
    Iterator &operator++() {
      _ptr = _ptr->_next;
      return *this;
    }
    // friend operator: it++
    friend Iterator operator++(Iterator &it, int unused) {
      it._ptr = it._ptr->_next;
      return it;
    }

   private:
    node<T> *_ptr;  // pointer being encapsulated
  };

  // constructor: CTOR
  Stack() : _top(nullptr), _size(0) {}

  // BIG 3:
  Stack(const Stack<T> &copyMe) { _top = _copy_list(copyMe._top); }

  ~Stack() { _clear_list(_top); }

  Stack<T> &operator=(const Stack<T> &RHS) { _top = _copy_list(RHS._top); }

  // Operations:
  void push(T item) {
    _insert_head(_top, item);
    _size++;
  }

  void pop() {
    if (!empty()) {
      _delete_node(_top, _top);
      _size--;
    }
  }

  // Accessors:
  Iterator begin() const { return Iterator(_top); }

  Iterator end() const { return Iterator(nullptr); }

  // Checkers:
  int size() const { return _size; }

  bool empty() { return _size == 0; }

  T top() {
    if (!empty()) {
      return _top->_item;
    }

    return T();
  }

  template <typename U>
  friend ostream &operator<<(ostream &outs, const Stack<U> &printMe);

 private:
  node<T> *_top;
  int _size;
};

// Definition

template <typename U>
ostream &operator<<(ostream &outs, const Stack<U> &printMe) {
  _print_list(printMe._top);
  return outs;
}

// TODO

#endif  // MYSTACK_H