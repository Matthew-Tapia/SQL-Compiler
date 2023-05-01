#ifndef MMAP_H
#define MMAP_H

#include <cstdlib> // Provides std::size_t
#include <iostream>

#include "bplustree.h"
#include "mpair.h"

template <typename K, typename V> class MMap {
public:
  // TYPEDEFS and MEMBER CONSTANTS
  typedef BPlusTree<MPair<K, V>> map_base;
  // NESTED CLASS DEFINITION
  class Iterator {
  public:
    friend class MMap;
    // CONSTRUCTORS

    Iterator(typename map_base::Iterator it = nullptr) { _it = it; }

    // OPERATORS

    Iterator operator++(int unused) { return ++_it; }

    Iterator operator++() { return _it++; }

    MPair<K, V> operator*() { return *_it; }

    friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
      return lhs._it == rhs._it;
    }

    friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
      return !(lhs == rhs);
    }

  private:
    typename map_base::Iterator _it;
  };

  // CONSTRUCTORS
  MMap() {}

  // ITERATORS
  Iterator begin() { return Iterator(mmap.begin()); }

  Iterator end() { return Iterator(mmap.end()); }

  // CAPACITY
  std::size_t size() { return mmap.size(); }

  std::size_t size_list() { return mmap.size_list(); }

  std::size_t count() const { return mmap.count(); }

  bool empty() const { return mmap.empty(); }

  // ELEMENT ACCESS

  const std::vector<V> &operator[](const K &key) const { return get(key); }

  std::vector<V> &operator[](const K &key) { return get(key); }

  std::vector<V> &at(const K &key) { return get(key); }

  const std::vector<V> &at(const K &key) const { return get(key); }

  // MODIFIERS
  void insert(const K &k, const V &v) {
    MPair<K, V> temp(k, v);
    mmap.insert(temp);
  }

  void erase(const K &key) {
    MPair<K, V> temp(key);
    mmap.erase(temp);
  }

  void clear() { mmap.clear_tree(); }

  // OPERATIONS

  Iterator find(const K &key) {
    MPair<K, V> temp(key);
    return mmap.find(temp);
  }

  bool contains(const K &key) const {
    MPair<K, V> temp(key);
    return mmap.contains(temp);
  }

  std::vector<V> &get(const K &key) {
    MPair<K, V> temp(key);
    if (!mmap.contains(temp)) {
      mmap.insert(temp);
    }
    return mmap.get(temp).value_list;
  }

  Iterator lower_bound(const K &key) { return Iterator(mmap.lower_bound(key)); }

  Iterator upper_bound(const K &key) { return Iterator(mmap.upper_bound(key)); }

  bool is_valid() { return mmap.is_valid(); }

  // OVERLOADED OPERATORS
  friend std::ostream &operator<<(std::ostream &outs,
                                  const MMap<K, V> &print_me) {
    outs << print_me.mmap << std::endl;
    return outs;
  }

  void print_lookup() {
    if (mmap.empty()) {
      return;
    }

    Iterator walker = begin();

    while (walker != end()) {
      std::cout << std::setw(10) << *(walker) << std::endl;

      ++walker;
    }
  }

private:
  BPlusTree<MPair<K, V>> mmap;
};

#endif // MMAP_H