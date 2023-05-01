#ifndef MAP_H
#define MAP_H

#include <cstdlib> // Provides std::size_t

#include "bplustree.h"
#include "pair.h"

template <typename K, typename V> class Map {
public:
  // TYPEDEFS and MEMBER CONSTANTS
  typedef BPlusTree<Pair<K, V>> map_base;
  // NESTED CLASS DEFINITION
  class Iterator {
  public:
    friend class Map;
    // CONSTRUCTORS
    Iterator(typename map_base::Iterator it = nullptr) { _it = it; }

    // OPERATORS
    Iterator operator++(int unused) { return ++_it; }

    Iterator operator++() { return _it++; }

    Pair<K, V> operator*() { return *_it; }

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
  Map() {}

  // ITERATORS
  Iterator begin() { return Iterator(map.begin()); }

  Iterator end() { return Iterator(map.end()); }

  // CAPACITY
  std::size_t size() { return map.size(); }

  std::size_t count() const { return map.count(); }

  bool empty() const { return map.empty(); }

  // ELEMENT ACCESS
  V &operator[](const K &key) {
    Pair<K, V> temp(key);
    map.insert(temp);
    return map.get(temp).value;
  }

  const V &operator[](const K &key) const {
    Pair<K, V> temp(key);
    map.insert(temp);
    return map.get(temp).value;
  }

  V &at(const K &key) { return map.get(key).value; }

  const V &at(const K &key) const { return get(key); }

  // MODIFIERS
  void insert(const K &k, const V &v) {
    Pair<K, V> temp(k, v);
    map.insert(temp);
  }
  void erase(const K &key) {
    Pair<K, V> temp(key);
    map.erase(temp);
  }

  void clear() { map.clear_tree(); }

  // OPERATIONS
  Iterator find(const K &key) {
    Pair<K, V> temp(key);
    return map.find(temp);
  }

  bool contains(const Pair<K, V> &target) const { return map.contains(target); }

  V &get(const K &key) {
    Pair<K, V> temp(key);
    return map.get(temp).value;
  }

  Iterator lower_bound(const K &key) { return map.lower_bound(key); }

  Iterator upper_bound(const K &key) { return map.upper_bound(key); }

  bool is_valid() { return map.is_valid(); }
  // OVERLOADED OPERATORS
  friend std::ostream &operator<<(std::ostream &outs,
                                  const Map<K, V> &print_me) {
    outs << print_me.map << std::endl;
    return outs;
  }

private:
  BPlusTree<Pair<K, V>> map;
};

#endif // MAP_H