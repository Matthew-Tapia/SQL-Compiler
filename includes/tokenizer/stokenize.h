#ifndef STOKENIZE_H
#define STOKENIZE_H

#include <cassert>
#include <cstring>
#include <iostream>

#include "constants.h"
#include "state_machine_functions.h"
#include "token.h"

class STokenizer {
public:
  // Constructors
  STokenizer() {
    init_table(_table);
    make_table(_table);
    _pos = 0;
    std::strcpy(_buffer, "");
  }

  STokenizer(const char str[]) {
    init_table(_table);
    make_table(_table);
    _pos = 0;
    std::strcpy(_buffer, str);
  }

  // true: there are no more tokens
  bool done() {
    if (_pos == strlen(_buffer)) {
      ++_pos;
      return false;
    }
    return (_pos >= strlen(_buffer));
  }

  // true: there are more tokens
  bool more() { return !done(); }

  // extract one token (very similar to the way cin >> works)
  friend STokenizer &operator>>(STokenizer &s, SToken &t) {
    s.get_token(s.posToStartState(), t);
    return s;
  }

  // set a new string as the input string
  void set_string(const char str[]) {
    _pos = 0;
    std::strcpy(_buffer, str);
  }

private:
  // create table for all the tokens we will recognize (e.g. doubles, words,
  // etc.)
  void make_table(int _table[][MAX_COLUMNS]) {
    for (int i = 0; i <= 12; ++i) {
      mark_success(_table, i);
    }

    mark_fail(_table, 1);
    mark_fail(_table, 4);
    mark_fail(_table, 8);
    mark_fail(_table, 9);

    // NUMBER
    mark_cells(0, _table, DIGITS, 0);
    mark_cell(0, _table, '.', 1);
    mark_cells(1, _table, DIGITS, 2);
    mark_cells(2, _table, DIGITS, 2);

    // ALPHA
    mark_cells(3, _table, ALFA, 3);
    mark_cells(3, _table, DIGITS, 3);
    mark_cell(3, _table, '-', 3);

    mark_cell(4, _table, '_', 4);
    mark_cells(4, _table, ALFA, 3);
    mark_cells(4, _table, DIGITS, 3);

    // SPACE
    mark_cells(5, _table, SPACES, 5);

    // OPERATORS
    mark_cells(6, _table, OPERATORS, 6);

    // COMMAS

    // STRING
    mark_cells(8, _table, 0, 256, 9);
    mark_cells(9, _table, 0, 256, 9);
    mark_cell(9, _table, '\"', 10);

    // PARENTH

    // STAR

    mark_success(_table, 10);
  }

  // extract the longest string that match one of the acceptable token types
  bool get_token(int start_state, SToken &token) {

    if (done() || _buffer[_pos] == '\0') {
      return false;
    }

    int initial_position = _pos;

    int last_successful_index = _pos;
    int last_successful_state = start_state;

    int current_state = start_state;

    while (_buffer[_pos] > 0 && current_state != -1 && _pos <= MAX_BUFFER &&
           _buffer[_pos] != '\0') {

      char currentChar = _buffer[_pos];

      current_state = _table[current_state][_buffer[_pos]];

      if (current_state == 10) {
        std::cout << "";
      }

      if (is_success(_table, current_state)) {
        last_successful_state = current_state;
        last_successful_index = _pos;
      }

      ++_pos;
    }

    _pos = last_successful_index + 1;

    STRING_TOKEN_TYPES result_type = token_type(last_successful_state);
    string result_str = "";

    if (result_type != TOKEN_UNKNOWN) {
      for (int i = initial_position; i <= last_successful_index; ++i) {
        result_str += _buffer[i];
      }
    }

    SToken temp(result_str, result_type);
    token = temp;

    return true;
  }

  // return the type of the token
  STRING_TOKEN_TYPES token_type(int state) const {
    switch (state) {
    case 0:
      return TOKEN_NUMBER;

    case 2:
      return TOKEN_NUMBER;

    case 3:
      return TOKEN_ALPHA;

    case 5:
      return TOKEN_SPACE;

    case 6:
      return TOKEN_OPERATOR;

    case 7:
      return TOKEN_COMMA;

    case 10:
      return TOKEN_STRING;

    case 11:
      return TOKEN_PARENTH;

    case 12:
      return TOKEN_STAR;

    default:
      return TOKEN_UNKNOWN;
    }
  }

  bool contains(const char arr[], char ch) {
    for (int i = 0; i < strlen(arr); ++i) {
      if (arr[i] == ch) {
        return true;
      }
    }

    return false;
  }

  int posToStartState() {
    char ch = _buffer[_pos];

    if (contains(DIGITS, ch)) {
      return 0;
    }

    if (contains(ALFA, ch)) {
      return 3;
    }

    if (ch == '_') {
      return 4;
    }

    if (contains(SPACES, ch)) {
      return 5;
    }

    if (contains(OPERATORS, ch)) {
      return 6;
    }

    if (ch == ',') {
      return 7;
    }

    if (ch == '\"') {
      return 8;
    }

    if (ch == '(' || ch == ')') {
      return 11;
    }

    if (ch == '*') {
      return 12;
    }

    return -1;
  }

  char _buffer[MAX_BUFFER + 1]; // input string
  int _pos;                     // current position in the string
  static int _table[MAX_ROWS][MAX_COLUMNS];
};

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS] = {0};

#endif // STOKENIZE_H