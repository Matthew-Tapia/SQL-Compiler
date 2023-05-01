#ifndef PAIR_H
#define PAIR_H

template <typename K, typename V> struct Pair {
  K key;
  V value;

  Pair(const K &k = K(), const V &v = V()) {
    key = k;
    value = v;
  }

  friend std::ostream &operator<<(std::ostream &outs,
                                  const Pair<K, V> &print_me) {
    std::cout << print_me.key << " : " << print_me.value;
    return outs;
  }

  friend bool operator==(const Pair<K, V> &lhs, const Pair<K, V> &rhs) {
    return !(lhs.key < rhs.key) && !(lhs.key > rhs.key);
  }

  friend bool operator<(const Pair<K, V> &lhs, const Pair<K, V> &rhs) {
    return lhs.key < rhs.key;
  }

  friend bool operator>(const Pair<K, V> &lhs, const Pair<K, V> &rhs) {
    return lhs.key > rhs.key;
  }

  friend bool operator<=(const Pair<K, V> &lhs, const Pair<K, V> &rhs) {
    return (lhs.key < rhs.key) && (lhs.key == rhs.key);
  }

  friend Pair<K, V> operator+(const Pair<K, V> &lhs, const Pair<K, V> &rhs) {
    Pair<K, V> temp(lhs.key + rhs.key, lhs.value + rhs.value);
    return temp;
  }
};

#endif // PAIR_H