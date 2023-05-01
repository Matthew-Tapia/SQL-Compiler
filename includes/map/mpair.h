#ifndef MPAIR_H
#define MPAIR_H

#include <cstdlib> // Provides std::size_t
#include <vector>  // Provides std::vector

template <typename K, typename V> struct MPair {
  K key;
  std::vector<V> value_list;

  //--------------------------------------------------------------------------------
  /*      MPair CTORs:
   *  must have these CTORs:
   *  - default CTOR / CTOR with a key and no value: this will create an empty
   * vector
   *  - CTOR with a key AND a value: pushes the value into the value_list
   *  - CTOR with a key and a vector of values (to replace value_list)
   */
  //--------------------------------------------------------------------------------
  MPair(const K &k = K()) { key = k; }

  MPair(const K &k, const V &v) {
    key = k;
    value_list.push_back(v);
  }

  MPair(const K &k, const std::vector<V> &vlist) {
    key = k;

    for (int i = 0; i < vlist.size(); ++i) {
      value_list.push_back(vlist[i]);
    }
  }
  //--------------------------------------------------------------------------------

  // Overloaded operators
  friend std::ostream &operator<<(std::ostream &outs,
                                  const MPair<K, V> &print_me) {
    outs << print_me.key << " : [";

    for (int i = 0; i < print_me.value_list.size(); ++i) {
      outs << " " << print_me.value_list[i];
    }

    outs << " ]";

    return outs;
  }
  friend bool operator==(const MPair<K, V> &lhs, const MPair<K, V> &rhs) {
    return lhs.key == rhs.key;
  }

  friend bool operator<(const MPair<K, V> &lhs, const MPair<K, V> &rhs) {
    return lhs.key < rhs.key;
  }

  friend bool operator<=(const MPair<K, V> &lhs, const MPair<K, V> &rhs) {
    return lhs.key <= rhs.key;
  }

  friend bool operator>(const MPair<K, V> &lhs, const MPair<K, V> &rhs) {
    return lhs.key > rhs.key;
  }

  friend MPair<K, V> operator+(MPair<K, V> &lhs, const MPair<K, V> &rhs) {
    MPair<K, V> temp(lhs.key);

    for (int i = 0; i < lhs.value_list.size(); ++i) {
      temp.value_list.push_back(lhs.value_list[i]);
    }

    for (int i = 0; i < rhs.value_list.size(); ++i) {
      temp.value_list.push_back(rhs.value_list[i]);
    }

    return temp;
  }
};

#endif // MPAIR_H