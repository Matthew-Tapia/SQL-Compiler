#ifndef LINKED_LIST_FUNCTIONS_H
#define LINKED_LIST_FUNCTIONS_H

#include <cassert>

#include "../node/node.h"

using namespace std;

// Declaration
// Linked List General Functions:
template <typename T>
void _print_list(node<T>* head) {
  struct node<T>* walker = head;

  cout << "Head->";
  while (walker != nullptr) {
    cout << *walker;
    walker = walker->_next;
  }
  cout << "|||\n";
}

// recursive fun! :)
template <typename T>
void _print_list_backwards(node<T>* head) {
  if (head == nullptr) {
    return;
  }

  _print_list_backwards(head->_next);

  cout << *head;
}

// return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key) {
  struct node<T>* walker = head;

  while (walker != nullptr && walker->_item != key) {
    walker = walker->_next;
  }

  return walker;
}

template <typename T>
node<T>* _insert_head(node<T>*& head, T insert_this) {
  struct node<T>* n = new node<T>;

  n->_item = insert_this;
  n->_next = head;
  n->_prev = nullptr;

  if (head != nullptr) {
    head->_prev = n;
  }

  head = n;

  return head;
}

// insert after ptr
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T>* after_this, T insert_this) {
  if (after_this != nullptr) {
    struct node<T>* n = new node<T>;

    n->_item = insert_this;

    n->_next = after_this->_next;
    after_this->_next = n;
    n->_prev = after_this;

    if (n->_next != nullptr) {
      n->_next->_prev = n;
    }

    return n;
  }

  return nullptr;
}

// insert before ptr
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this) {
  if (before_this != nullptr) {
    struct node<T>* n = new node<T>;

    n->_item = insert_this;

    n->_next = before_this;
    n->_prev = before_this->_prev;
    before_this->_prev = n;

    if (n->_prev != nullptr) {
      n->_prev->_next = n;
    } else {
      head = n;
    }

    return n;
  }

  return nullptr;
}

// ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* head, node<T>* prev_to_this) {
  struct node<T>* walker = head;

  while (walker->_next != nullptr && walker->_next != prev_to_this) {
    walker = walker->_next;
  }

  return walker;
}

// delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this) {
  T result = delete_this->_item;

  if (delete_this == head) {
    head = delete_this->_next;
  }

  if (delete_this->_next != nullptr) {
    delete_this->_next->_prev = delete_this->_prev;
  }

  if (delete_this->_prev != nullptr) {
    delete_this->_prev->_next = delete_this->_next;
  }

  free(delete_this);

  return result;
}

// duplicate the list
template <typename T>
node<T>* _copy_list(node<T>* head) {
  if (head == nullptr) {
    return head;
  }

  struct node<T>* n = new node<T>;
  n->_item = head->_item;
  n->_next = _copy_list(head->_next);

  return n;
}

// duplicate list and return the last node of the copy
template <typename T>
node<T>* _copy_list(node<T>*& dest, node<T>* src) {
  dest = _copy_list(src);
  return _last_node(dest);
}

// delete all the nodes
template <typename T>
void _clear_list(node<T>*& head) {
  struct node<T>* walker = head;

  while (walker != nullptr) {
    head = head->_next;
    free(walker);
    walker = head;
  }
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos) {
  struct node<T>* walker = head;

  for (int i = 0; i < pos && walker != nullptr; i++) {
    walker = walker->_next;
  }

  return walker->_item;
}

// Last node in the list, never use this function.
template <typename T>
node<T>* _last_node(node<T>* head) {
  struct node<T>* walker = head;

  while (walker->_next != nullptr) {
    walker = walker->_next;
  }

  return walker;
}

// Definition

// TODO

#endif  // LINKED_LIST_FUNCTIONS_H