#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <cassert>  // Provides assert
#include <cstdlib>  // Provides size_t
#include <iomanip>  // Provides std::setw
#include <iostream> // Provides std::cout
#include <string>   // Provides std::to_string

#include "btree_array_functions.h" // Include the implementation.

template <class Item> class BPlusTree {
public:
  // TYPEDEFS and MEMBER CONSTANTS
  typedef Item value_type;
  // NESTED CLASS DEFINITION
  class Iterator {
  public:
    friend class BPlusTree;
    // CONSTRUCTORS

    Iterator(BPlusTree *_it = nullptr, std::size_t _key_ptr = 0)
        : it(_it), key_ptr(_key_ptr) {}

    // MODIFICATION MEMBER FUNCTIONS

    Item operator*() {
      return it->data_count == 0 ? Item() : it->data[key_ptr];
    }

    const Item operator*() const {
      return it->data_count == 0 ? Item() : it->data[key_ptr];
    }

    Iterator operator++(int un_used) {
      Iterator temp = *this;

      ++key_ptr;
      if (key_ptr == it->data_count) {
        key_ptr = 0;
        it = it->next;
      }

      return temp;
    }

    Iterator operator++() {
      ++key_ptr;
      if (key_ptr == it->data_count) {
        key_ptr = 0;
        it = it->next;
      }

      return *this;
    }
    // OVERLOADED OPERATORS

    friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
      return lhs.it == rhs.it;
    }

    friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
      return !(lhs == rhs);
    }

    // HELPER FUNCTIONS

    bool is_null() { return !it; }

    void print_Iterator() {
      std::cout << "Iterator: " << it << " key_ptr: " << key_ptr << std::endl;
    }

    void info() const {
      std::cout << "|";
      for (int i = 0; i < it->data_count; ++i) {
        std::cout << it->data[i] << "|";
      }

      std::cout << std::endl;
    }

  private:
    BPlusTree<Item> *it;
    std::size_t key_ptr;
  };

  // CONSTRUCTORS and DESTRUCTOR
  BPlusTree() {
    data_count = 0;
    child_count = 0;
    next = nullptr;
  }

  BPlusTree(const BPlusTree &source) { copy_tree(source); }

  BPlusTree(Item *a, std::size_t size) {
    data_count = 0;
    child_count = 0;
    next = nullptr;

    for (int i = 0; i < size; ++i) {
      insert(a[i]);
    }
  }

  ~BPlusTree() { clear_tree(); }

  // MODIFICATION MEMBER FUNCTIONS
  BPlusTree &operator=(const BPlusTree &source) {
    copy_tree(source);
    return *this;
  }

  void clear_tree() {
    for (int i = 0; i < child_count; ++i) {
      subset[i]->clear_tree();
      delete subset[i];
    }
    child_count = 0;
    data_count = 0;
    next = nullptr;
  }

  void copy_tree(const BPlusTree &source) {
    BPlusTree *temp = new BPlusTree();
    copy_tree(source, temp);

    temp = nullptr;
    delete temp;
  }

  void copy_tree(const BPlusTree &source, BPlusTree *&last_node) {
    data_count = source.data_count;
    child_count = source.child_count;

    if (is_leaf()) {
      last_node->next = this;
      last_node = this;
    }

    next = nullptr;

    copy_array(data, source.data, data_count, source.data_count);

    for (int i = 0; i < source.child_count; ++i) {
      BPlusTree *temp = new BPlusTree();

      subset[i] = temp;
      subset[i]->copy_tree(*source.subset[i], last_node);
    }
  }

  bool insert(const Item &entry) {
    {

      if (!loose_insert(entry)) {
        return false;
      }

      if (data_count > MAXIMUM) {
        BPlusTree *temp = new BPlusTree();

        copy_array(temp->data, data, temp->data_count, data_count);
        copy_array(temp->subset, subset, temp->child_count, child_count);

        data_count = 0;
        child_count = 0;
        next = nullptr;

        insert_item(subset, 0, child_count, temp);
        fix_excess(0);
      }

      return true;
    }
  }

  bool erase(const Item &target) {
    if (!loose_erase(target)) {
      return false;
    }

    if (data_count == 0 && child_count == 1) {
      BPlusTree *temp = subset[0];

      copy_array(data, temp->data, data_count, temp->data_count);
      copy_array(subset, temp->subset, child_count, temp->child_count);

      temp->child_count = 0;
      delete temp;
    }

    return true;
  }

  // NON-CONSTANT MEMBER FUNCTIONS
  Iterator find(const Item &entry) {
    int i = first_ge(data, data_count, entry);

    if (is_leaf()) {
      return i < data_count && !(data[i] > entry) ? Iterator(this, i)
                                                  : Iterator();
    } else {
      return i < data_count && !(data[i] > entry) ? subset[i + 1]->find(entry)
                                                  : subset[i]->find(entry);
    }
  }

  Item &get(const Item &entry) {
    Iterator temp = find(entry);
    return temp.it->data[temp.key_ptr];
  }

  std::size_t size() {
    BPlusTree *walker = get_smallest_node();
    int counter = 0;

    while (walker != nullptr) {
      counter += walker->data_count;
      walker = walker->next;
    }

    return counter;
  }

  std::size_t size_list() {
    BPlusTree *walker = get_smallest_node();
    int counter = 0;

    while (walker != nullptr) {

      for (int i = 0; i < data_count; ++i) {
        counter += walker->data[i].value_list.size();
      }

      walker = walker->next;
    }

    return counter;
  }

  Iterator lower_bound(const Item &key) {
    Iterator walker = begin();

    while (walker != end()) {
      if (!(*walker < key)) {
        return walker;
      }
      ++walker;
    }

    return Iterator();
  }

  Iterator upper_bound(const Item &key) {
    Iterator walker = begin();

    while (walker != end()) {
      if (*walker > key) {
        return walker;
      }
      ++walker;
    }

    return Iterator();
  }

  Iterator begin() { return Iterator(get_smallest_node()); }

  Iterator end() { return Iterator(); }

  // CONSTANT MEMBER FUNCTIONS
  bool contains(const Item &entry) const {
    int i = first_ge(data, data_count, entry);

    if (i < data_count && !(data[i] > entry)) {
      return true;
    } else if (is_leaf()) {
      return false;
    } else {
      return subset[i]->contains(entry);
    }
  }

  const Item &get(const Item &entry) const { return *find(entry); }

  std::size_t count() const {
    if (is_leaf()) {
      return data_count;
    }

    int sum = data_count;

    for (int i = 0; i < child_count; ++i) {
      sum += subset[i]->count();
    }

    return sum;
  }

  bool empty() const { return (data_count == 0 && is_leaf()); }

  void print(int indent = 0, std::ostream &outs = std::cout) const {
    std::string down_bracket = "\357\271\207"; // ﹇
    std::string up_bracket = "\357\271\210";   // ﹈
    std::string indent_string = std::string(indent * 4, ' ');

    if (is_leaf()) {
      std::cout << indent_string << "^" << std::endl;
      std::cout << indent_string << down_bracket << std::endl;

      for (int i = data_count - 1; i >= 0; --i) {
        std::cout << indent_string << data[i] << std::endl;
      }

      std::cout << indent_string << up_bracket << std::endl;
    } else {
      int i = data_count;
      for (; i > 0; i--) {
        subset[i]->print(indent + 1);
        if (i == data_count) {
          std::cout << indent_string << down_bracket << std::endl;
        }
        std::cout << indent_string << data[i - 1] << std::endl;
      }
      std::cout << indent_string << up_bracket << std::endl;
      subset[i]->print(indent + 1);
    }
  }

  bool is_valid() const {
    bool valid = true;
    // check if the node is empty
    if (empty())
      return true;
    // check if the node has too many entries
    if (data_count > MAXIMUM || data_count < MINIMUM) {
      return false;
    }
    // check if the node has too many children
    if (child_count > MAXIMUM + 1 || child_count < 0) {
      return false;
    }
    // check if the data is sorted
    for (size_t i = 0; i < data_count - 1; i++) {
      if (data[i] > data[i + 1])
        return false;
    }
    if (!is_leaf()) {
      // check if the child_count is not equal to data_count+1
      if (child_count != data_count + 1) {
        return false;
      }
      // check if data is in range of children
      for (size_t i = 0; i < data_count; i++) {
        // check if data[i] is greater than subset[i]
        valid = is_gt(subset[i]->data, subset[i]->data_count, data[i]);
        if (!valid)
          return false;
        // check if data[i] is less than subset[i+1]
        valid = is_le(subset[i + 1]->data, subset[i + 1]->data_count, data[i]);
        if (!valid)
          return false;
        // check if subset[i] is valid
        valid = subset[i]->is_valid();
        if (!valid)
          return false;
        // check if data[i] is the smallest node in subset[i+1]
        valid = data[i] == subset[i + 1]->get_smallest_node()->data[0];
        if (!valid)
          return false;
      }
      // check if the last child is valid
      valid = subset[data_count]->is_valid();
      if (!valid)
        return false;
    }
    return true;
  }

  // OVERLOAD OPERATOR FUNCTIONS
  friend std::ostream &operator<<(std::ostream &outs,
                                  const BPlusTree<Item> &btree) {
    btree.print(0, outs);
    return outs;
  }
  // SUGGESTED FUNCTION FOR DEBUGGING
  std::string in_order() {
    std::string result = "";

    if (is_leaf()) {
      for (int i = 0; i < data_count; ++i) {
        result += std::to_string(data[i]) + "|";
      }
    } else {
      for (int i = 0; i < data_count; ++i) {
        result += subset[i]->in_order();
        result += std::to_string(data[i]) + "|";
      }

      result += subset[data_count]->in_order();
    }

    return result;
  }

  std::string pre_order() {
    std::string result = "";

    if (is_leaf()) {
      for (int i = 0; i < data_count; ++i) {
        result += std::to_string(data[i]) + "|";
      }
    } else {
      for (int i = 0; i < data_count; ++i) {
        result += std::to_string(data[i]) + "|";
        result += subset[i]->in_order();
      }

      result += subset[data_count]->in_order();
    }

    return result;
  }

  std::string post_order() {
    std::string result = "";

    if (is_leaf()) {
      for (int i = 0; i < data_count; ++i) {
        result += std::to_string(data[i]) + "|";
      }
    } else {
      for (int i = 0; i < data_count; ++i) {

        if (i == 0) {
          result += subset[i]->in_order();
        }
        result += subset[i + 1]->in_order();
        result += std::to_string(data[i]) + "|";
      }
    }

    return result;
  }

private:
  // MEMBER CONSTANTS
  static const std::size_t MINIMUM = 1;
  static const std::size_t MAXIMUM = 2 * MINIMUM;
  // MEMBER VARIABLES
  std::size_t data_count;
  Item data[MAXIMUM + 1];
  std::size_t child_count;
  BPlusTree *subset[MAXIMUM + 2];
  BPlusTree *next;

  // HELPER MEMBER FUNCTIONS

  bool is_leaf() const { return (child_count == 0); }

  BPlusTree<Item> *get_smallest_node() {
    if (is_leaf()) {
      return this;
    }

    return subset[0]->get_smallest_node();
  }

  // insert element functions

  bool loose_insert(const Item &entry) {
    int i = first_ge(data, data_count, entry);

    if (is_leaf() && i < data_count && !(data[i] > entry)) {
      return false;
    }

    if (!is_leaf() && i < data_count && !(entry < data[i])) {
      bool temp = subset[i + 1]->loose_insert(entry);
      if (subset[i + 1]->data_count > MAXIMUM) {
        fix_excess(i + 1);
      }

      return temp;
    }

    if (is_leaf()) {
      ordered_insert(data, data_count, entry);
      return true;
    } else {
      bool temp = subset[i]->loose_insert(entry);
      if (subset[i]->data_count > MAXIMUM) {
        fix_excess(i);
      }

      return temp;
    }
  }

  void fix_excess(std::size_t i) {
    BPlusTree *temp = new BPlusTree();

    insert_item(subset, i + 1, child_count, temp);

    split(subset[i]->data, subset[i]->data_count, temp->data, temp->data_count);
    split(subset[i]->subset, subset[i]->child_count, temp->subset,
          temp->child_count);

    Item temp_entry;
    detach_item(subset[i]->data, subset[i]->data_count, temp_entry);

    if (subset[i]->is_leaf()) {
      temp->next = subset[i]->next;
      subset[i]->next = temp;
      ordered_insert(temp->data, temp->data_count, temp_entry);
    }

    ordered_insert(data, data_count, temp_entry);
  }

  // remove element functions

  bool loose_erase(const Item &target) {
    int i = first_ge(data, data_count, target);

    // NOT-FOUND LEAF
    if (is_leaf() && i == data_count) {
      return false;
    }

    // FOUND LEAF
    if (is_leaf() && i < data_count && !(data[i] > target)) {
      Item temp;
      delete_item(data, i, data_count, temp);
      return true;
    }

    // NOT-FOUND NOT-LEAF
    if (!is_leaf() && (i == data_count || data[i] > target)) {
      bool temp = subset[i]->loose_erase(target);

      fix_shortage(i);

      return temp;
    }

    // FOUND NOT-LEAF
    if (!is_leaf() && i < data_count && !(data[i] > target)) {
      bool temp_bool = subset[i + 1]->loose_erase(target);
      Iterator temp = upper_bound(target);

      fix_shortage(i + 1);

      delete_internal_node(i, target, *temp);

      return temp_bool;
    }

    return false;
  }

  void fix_shortage(std::size_t i) {
    if (subset[i]->data_count >= MINIMUM) {
      return;
    }

    if (i > 0 && subset[i - 1]->data_count > MINIMUM) {
      transfer_from_left(i);
      return;
    }

    if (i < child_count - 1 && subset[i + 1]->data_count > MINIMUM) {
      transfer_from_right(i);
      return;
    }

    if (i > 0 && subset[i - 1]->data_count <= MINIMUM) {
      Item temp;

      delete_item(data, i - 1, data_count, temp);
      merge_with_next_subset(i - 1);

      if (!subset[i - 1]->is_leaf()) {
        ordered_insert(subset[i - 1]->data, subset[i - 1]->data_count, temp);
      }

      return;
    }

    if (i < child_count - 1 && subset[i + 1]->data_count <= MINIMUM) {
      Item temp;

      delete_item(data, i, data_count, temp);

      merge_with_next_subset(i);

      if (!subset[i]->is_leaf()) {
        ordered_insert(subset[i]->data, subset[i]->data_count, temp);
      }

      return;
    }
  }

  void delete_internal_node(std::size_t i, const Item &target,
                            const Item &next_smallest) {
    int j = first_ge(data, data_count, target);

    if (j < data_count && !(data[j] > target)) {
      data[j] = next_smallest;
      return;
    }

    j = first_ge(subset[i]->data, subset[i]->data_count, target);

    if (j < subset[i]->data_count && !(subset[i]->data[j] > target)) {
      subset[i]->data[j] = next_smallest;
      return;
    }

    j = first_ge(subset[i + 1]->data, subset[i + 1]->data_count, target);

    if (j < subset[i + 1]->data_count && !(subset[i + 1]->data[j] > target)) {
      subset[i + 1]->data[j] = next_smallest;
      return;
    }
  }

  void transfer_from_left(std::size_t i) {
    Item temp;

    if (subset[i]->is_leaf()) {
      detach_item(subset[i - 1]->data, subset[i - 1]->data_count, temp);
      ordered_insert(subset[i]->data, subset[i]->data_count, temp);

      data[i - 1] = temp;

      return;
    }

    delete_item(data, i - 1, data_count, temp);
    insert_item(subset[i]->data, 0, subset[i]->data_count, temp);
    detach_item(subset[i - 1]->data, subset[i - 1]->data_count, temp);
    insert_item(data, i - 1, data_count, temp);

    BPlusTree *temp_tree;

    detach_item(subset[i - 1]->subset, subset[i - 1]->child_count, temp_tree);
    insert_item(subset[i]->subset, 0, subset[i]->child_count, temp_tree);
  }

  void transfer_from_right(std::size_t i) {
    Item temp;

    if (subset[i]->is_leaf()) {
      delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
      ordered_insert(subset[i]->data, subset[i]->data_count, temp);

      data[i] = subset[i + 1]->data[0];

      if (i > 0) {
        data[i - 1] = subset[i]->data[0];
      }

      return;
    }

    delete_item(data, i, data_count, temp);
    attach_item(subset[i]->data, subset[i]->data_count, temp);
    delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
    insert_item(data, i, data_count, temp);

    BPlusTree *temp_tree;

    delete_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count,
                temp_tree);
    attach_item(subset[i]->subset, subset[i]->child_count, temp_tree);
  }

  void merge_with_next_subset(std::size_t i) {
    merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data,
          subset[i + 1]->data_count);
    merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset,
          subset[i + 1]->child_count);

    BPlusTree *temp;

    if (subset[i]->is_leaf()) {
      subset[i]->next = subset[i + 1]->next;
    }

    delete_item(subset, i + 1, child_count, temp);
  }
};

// Implementation ITERATOR MEMBER FUNCTIONS

// TODO

// Implementation BPlusTree MEMBER FUNCTIONS

// TODO

#endif // BPLUSTREE_H