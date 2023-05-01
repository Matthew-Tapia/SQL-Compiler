#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <iomanip>
#include <iostream>

#include "../linked_list_functions/linked_list_functions.h"

using namespace std;

template <typename T>
class Queue {
 public:
  class Iterator {
   public:
    // Give access to list to access _ptr
    friend class Queue;

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
    bool is_null() { return (_ptr == nullptr); }

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
  Queue() : _front(nullptr), _rear(nullptr), _size(0) {}

  // BIG 3:
  Queue(const Queue<T> &copyMe) { _front = _copy_list(copyMe._front); }

  ~Queue() { _clear_list(_front); }

  Queue<T> &operator=(const Queue<T> &RHS) { _front = _copy_list(RHS._front); }

  // Operations:
  void push(T item) {
    if (_front == nullptr) {
      _insert_head(_front, item);
      _rear = _front;
    } else {
      _insert_after(_front, _rear, item);
      _rear = _rear->_next;
    }

    ++_size;
  }

  void pop() {
    if (!empty()) {
      _delete_node(_front, _front);
      --_size;
    }
  }

  // Accessors:
  Iterator begin() const { return Iterator(_front); }

  Iterator end() const { return Iterator(nullptr); }

  // Checkers:
  int size() const { return _size; }

  bool empty() { return _size == 0; }

  T front() {
    if (!empty()) {
      return _front->_item;
    }

    return T();
  }

  T back() {
    if (!empty()) {
      return _rear->_item;
    }

    return T();
  }

  template <typename U>
  friend ostream &operator<<(ostream &outs, const Queue<U> &printMe);

 private:
  node<T> *_front;
  node<T> *_rear;
  int _size;
};

// Definition

template <typename U>
ostream &operator<<(ostream &outs, const Queue<U> &printMe) {
  _print_list(printMe._front);
  return outs;
}

// TODO

#endif  // MYQUEUE_H