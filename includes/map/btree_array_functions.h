#ifndef BTREE_ARRAY_FUNCTIONS_H
#define BTREE_ARRAY_FUNCTIONS_H

#include <cassert>  // Provides assert
#include <cstdlib>  // Provides size_t
#include <iomanip>  // Provides std::setw
#include <iostream> // Provides std::cout
#include <vector>

// Declaration

template <class T> void swap(T &a, T &b) {
  T temp = a;
  a = b;
  b = temp;
}

template <class T> T maximal(const T &a, const T &b) { return a >= b ? a : b; }

template <class T> std::size_t index_of_maximal(T data[], std::size_t n) {
  if (n <= 0) {
    return -1;
  }

  int largestIndex = 0;

  for (int i = 1; i < n; ++i) {
    if (data[i] > data[largestIndex]) {
      largestIndex = i;
    }
  }

  return largestIndex;
}

template <class T>
std::size_t first_ge(const T data[], std::size_t n, const T &entry) {
  int i = 0;
  for (; i < n; ++i) {
    if (!(data[i] < entry)) {
      return i;
    }
  }

  return i;
}

template <class T>
void insert_item(T data[], std::size_t i, std::size_t &n, T entry) {
  for (int j = n; j > i; --j) {
    data[j] = data[j - 1];
  }

  data[i] = entry;
  ++n;
}

template <class T> void ordered_insert(T data[], std::size_t &n, T entry) {
  insert_item(data, first_ge(data, n, entry), n, entry);
}

template <class T> void attach_item(T data[], std::size_t &n, const T &entry) {
  data[n] = entry;
  ++n;
}

template <class T>
void delete_item(T data[], std::size_t i, std::size_t &n, T &entry) {
  entry = data[i];

  for (int j = i; j < n - 1; ++j) {
    data[j] = data[j + 1];
  }

  --n;
}

template <class T> void detach_item(T data[], std::size_t &n, T &entry) {
  entry = data[n - 1];
  --n;
}

template <class T>
void merge(T data1[], std::size_t &n1, T data2[], std::size_t &n2) {
  for (int i = 0; i < n2; ++i) {
    data1[n1 + i] = data2[i];
  }

  n1 = n1 + n2;
  n2 = 0;
}

template <class T>
void split(T data1[], std::size_t &n1, T data2[], std::size_t &n2) {
  int temp = (n1 / 2);

  for (int i = 0; i < temp; ++i) {
    data2[i] = data1[n1 - temp + i];
  }

  n1 -= temp;
  n2 = temp;
}

template <class T>
void copy_array(T dest[], const T src[], std::size_t &dest_size,
                std::size_t src_size) {
  for (int i = 0; i < src_size; ++i) {
    dest[i] = src[i];
  }

  dest_size = src_size;
}

template <class T> bool is_le(const T data[], std::size_t n, const T &item) {
  for (std::size_t i = 0; i < n; i++) {
    if (item > data[i])
      return false;
  }
  return true;
}

template <class T> bool is_gt(const T data[], std::size_t n, const T &item) {
  for (std::size_t i = 0; i < n; i++) {
    if (item <= data[i])
      return false;
  }
  return true;
}

template <class T>
void print_array(const T data[], std::size_t n, std::size_t pos = 0) {
  std::cout << "[ ";

  for (int i = pos; i < n; ++i) {
    std::cout << data[i] << " ";
  }

  std::cout << "]\n";
}

template <typename T>
std::vector<T> &operator+=(std::vector<T> &list, const T &addme) {
  list.push_back(addme);

  return list;
}

template <typename T>
std::ostream &operator<<(std::ostream &outs, std::vector<T> &list) {

  for (int i = 0; i < list.size(); ++i) {
    outs << list[i] << " ";
  }

  return outs;
}

// Definition

// TODO

#endif // BTREE_ARRAY_FUNCTIONS_H