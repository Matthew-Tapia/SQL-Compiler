#ifndef PARSE_H
#define PARSE_H

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

#include "../linked_list/queue/MyQueue.h"
#include "../map/map.h"
#include "../map/mmap.h"
#include "../tokenizer/sql_token.h"
#include "../tokenizer/stokenize.h"

class Parser {
public:
  // Constructors
  Parser() {}

  Parser(const char *&cstr) { _str = cstr; }

  Parser(const string &str) { _str = str; }

  // save string to private member
  void set_string(const char *&cstr) { _str = cstr; }

  void set_string(const string &str) { _str = str; }

  // parse string to map
  MMap<std::string, std::string> parse_tree() {
    cout << _str << endl;

    make_table();
    MMap<std::string, std::string> result;

    STokenizer stk(_str.c_str());
    SToken t;

    Queue<SToken> tokenQueue;
    Queue<SQLToken> sqlTokenQueue;

    // Tokenize String
    while (stk.more()) {
      // process token here...
      stk >> t;

      tokenQueue.push(t);

      t = SToken();
    }

    // Convert from String Tokens to SQL Tokens
    SQL_KEYWORDS current_keyword = tokenToSQLKeyWord(tokenQueue.front());
    std::string current_string = tokenQueue.front().token_str();

    while (tokenQueue.size() > 1) {

      // std::cout << setw(13) << current_string << " : "
      //           << tokenQueue.front().token_type() << std::endl;

      if (current_keyword == SQL_STRING) {
        current_keyword = SQL_WORD;
        current_string = current_string.substr(1, current_string.size() - 2);
      }

      SQLToken sqlT(current_string, current_keyword);

      // std::cout << setw(13) << sqlT.token_str() << " : " << sqlT.token_type()
      //           << std::endl
      //           << std::endl;

      if (sqlT.type() != SQL_SPACE) {
        sqlTokenQueue.push(sqlT);
      }
      tokenQueue.pop();
      current_keyword = tokenToSQLKeyWord(tokenQueue.front());
      current_string = tokenQueue.front().token_str();
    }

    if (sqlTokenQueue.front().type() == SQL_SELECT) {
      selectQueueToMMap(sqlTokenQueue, result);
    }

    if (sqlTokenQueue.front().type() == SQL_CREATE) {
      createQueueToMMap(sqlTokenQueue, result);
    }

    if (sqlTokenQueue.front().type() == SQL_INSERT) {
      insertQueueToMMap(sqlTokenQueue, result);
    }

    return result;
  }

  // =====================================
  // TODO: Add more methods here if needed
  // =====================================

  void make_table() {

    init_table(_keywordTable);

    // SELECT
    for (int i = 0; i <= 5; ++i) {
      mark_fail(_keywordTable, i);
    }

    mark_success(_keywordTable, 4);

    mark_cell(0, _keywordTable, SQL_TABLE, 1);
    mark_cell(1, _keywordTable, SQL_WORD, 2);
    mark_cell(2, _keywordTable, SQL_FIELDS, 3);
    mark_cell(3, _keywordTable, SQL_WORD, 4);
    mark_cell(4, _keywordTable, SQL_COMMA, 5);
    mark_cell(5, _keywordTable, SQL_WORD, 4);

    // INSERT
    for (int i = 6; i <= 11; ++i) {
      mark_fail(_keywordTable, i);
    }

    mark_success(_keywordTable, 10);

    mark_cell(6, _keywordTable, SQL_INTO, 7);
    mark_cell(7, _keywordTable, SQL_WORD, 8);
    mark_cell(8, _keywordTable, SQL_VALUES, 9);
    mark_cell(9, _keywordTable, SQL_WORD, 10);
    mark_cell(9, _keywordTable, SQL_STRING, 10);
    mark_cell(9, _keywordTable, SQL_NUMBER, 10);
    mark_cell(10, _keywordTable, SQL_COMMA, 11);
    mark_cell(11, _keywordTable, SQL_WORD, 10);
    mark_cell(11, _keywordTable, SQL_STRING, 10);
    mark_cell(11, _keywordTable, SQL_NUMBER, 10);

    // SELECT

    for (int i = 12; i <= 26; ++i) {
      mark_fail(_keywordTable, i);
    }

    mark_success(_keywordTable, 17);
    mark_success(_keywordTable, 20);
    mark_success(_keywordTable, 26);

    mark_cell(12, _keywordTable, SQL_STAR, 13);
    mark_cell(12, _keywordTable, SQL_WORD, 14);
    mark_cell(13, _keywordTable, SQL_FROM, 16);
    mark_cell(14, _keywordTable, SQL_COMMA, 15);
    mark_cell(14, _keywordTable, SQL_FROM, 16);
    mark_cell(15, _keywordTable, SQL_WORD, 14);
    mark_cell(16, _keywordTable, SQL_WORD, 17);
    mark_cell(17, _keywordTable, SQL_WHERE, 18);
    mark_cell(18, _keywordTable, SQL_NOT, 19);
    mark_cell(18, _keywordTable, SQL_WORD, 20);
    mark_cell(18, _keywordTable, SQL_LEFT_PARENTH, 22);
    mark_cell(19, _keywordTable, SQL_WORD, 20);
    mark_cell(20, _keywordTable, SQL_OPERATOR, 21);
    mark_cell(20, _keywordTable, SQL_AND, 18);
    mark_cell(20, _keywordTable, SQL_OR, 18);
    mark_cell(21, _keywordTable, SQL_WORD, 20);
    mark_cell(21, _keywordTable, SQL_STRING, 20);
    mark_cell(21, _keywordTable, SQL_NUMBER, 20);
    mark_cell(22, _keywordTable, SQL_NOT, 23);
    mark_cell(22, _keywordTable, SQL_WORD, 24);
    mark_cell(23, _keywordTable, SQL_WORD, 24);
    mark_cell(24, _keywordTable, SQL_AND, 22);
    mark_cell(24, _keywordTable, SQL_OR, 22);
    mark_cell(24, _keywordTable, SQL_OPERATOR, 25);
    mark_cell(24, _keywordTable, SQL_RIGHT_PARENT, 26);
    mark_cell(25, _keywordTable, SQL_WORD, 24);
    mark_cell(25, _keywordTable, SQL_NUMBER, 24);
    mark_cell(25, _keywordTable, SQL_STRING, 24);
    mark_cell(26, _keywordTable, SQL_AND, 18);
    mark_cell(26, _keywordTable, SQL_OR, 18);
  }

  SQL_KEYWORDS tokenToSQLKeyWord(SToken t) {
    std::string token_str = t.token_str();
    STRING_TOKEN_TYPES token_type = t.type();

    if (token_str == "create") {
      return SQL_CREATE;
    }

    if (token_str == "table") {
      return SQL_TABLE;
    }

    if (token_str == "fields") {
      return SQL_FIELDS;
    }

    if (token_str == "insert") {
      return SQL_INSERT;
    }

    if (token_str == "into") {
      return SQL_INTO;
    }

    if (token_str == "values") {
      return SQL_VALUES;
    }

    if (token_str == "select") {
      return SQL_SELECT;
    }

    if (token_str == "from") {
      return SQL_FROM;
    }

    if (token_str == "where") {
      return SQL_WHERE;
    }

    if (token_str == "and") {
      return SQL_AND;
    }

    if (token_str == "not") {
      return SQL_NOT;
    }

    if (token_str == "or") {
      return SQL_OR;
    }

    if (token_str == "*") {
      return SQL_STAR;
    }

    if (token_type == TOKEN_ALPHA) {
      return SQL_WORD;
    }

    if (token_type == TOKEN_NUMBER) {
      return SQL_NUMBER;
    }

    if (token_type == TOKEN_STRING) {
      return SQL_STRING;
    }

    if (token_type == TOKEN_COMMA) {
      return SQL_COMMA;
    }

    if (token_type == TOKEN_OPERATOR) {
      return SQL_OPERATOR;
    }

    if (token_type == TOKEN_PARENTH) {
      return token_str == "(" ? SQL_LEFT_PARENTH : SQL_RIGHT_PARENT;
    }

    if (token_type == TOKEN_SPACE) {
      return SQL_SPACE;
    }

    return SQL_UNKNOWN;
  }

  int SQLKeywordToStartState(SQL_KEYWORDS keyword) {
    if (SQL_CREATE) {
      return 0;
    }

    if (SQL_INSERT) {
      return 6;
    }

    if (SQL_SELECT) {
      return 12;
    }

    return -1;
  }

  bool selectQueueToMMap(Queue<SQLToken> &sqlTokenQueue,
                         MMap<std::string, std::string> &mmap) {

    int current_state = 12;
    mmap["command"] += std::string("select");
    sqlTokenQueue.pop();

    while (!sqlTokenQueue.empty() && current_state != -1) {

      std::string current_string = sqlTokenQueue.front().token_str();
      SQL_KEYWORDS current_keyword = sqlTokenQueue.front().type();

      if (current_state == 12 && current_keyword == SQL_STAR) {
        mmap["fields"] += current_string;
      }

      if ((current_state == 12 || current_state == 15) &&
          current_keyword == SQL_WORD) {
        mmap["fields"] += current_string;
      }

      if ((current_state == 12 || current_state == 15) &&
          current_keyword == SQL_STRING) {
        mmap["fields"] += current_string.substr(2, current_string.size() - 2);
      }

      if (current_state == 16 && current_keyword == SQL_WORD) {
        mmap["table_name"] += current_string;
      }

      if (current_state == 17 && current_keyword == SQL_WHERE) {
        mmap["where"] += std::string("yes");
      }

      if (current_state >= 18 && current_keyword == SQL_STRING) {
        mmap["condition"] +=
            current_string.substr(1, current_string.size() - 2);
      }

      if (current_state >= 18 && current_keyword != SQL_STRING) {
        mmap["condition"] += current_string;
      }

      current_state = _keywordTable[current_state][current_keyword];

      sqlTokenQueue.pop();
    }

    if (current_state == -1 && !is_success(_keywordTable, current_state)) {

      mmap.clear();
      return false;
    }

    return true;
  }

  bool createQueueToMMap(Queue<SQLToken> &sqlTokenQueue,
                         MMap<std::string, std::string> &mmap) {

    int current_state = 0;
    mmap["command"] += std::string("create");
    sqlTokenQueue.pop();

    while (!sqlTokenQueue.empty() && current_state != -1) {

      std::string current_string = sqlTokenQueue.front().token_str();
      SQL_KEYWORDS current_keyword = sqlTokenQueue.front().type();

      if (current_state == 1 && current_keyword == SQL_WORD) {
        mmap["table_name"] += current_string;
      }

      if ((current_state == 3 || current_state == 5) &&
          current_keyword == SQL_WORD) {
        mmap["fields"] += current_string;
      }

      if ((current_state == 3 || current_state == 5) &&
          current_keyword == SQL_STRING) {
        mmap["fields"] += current_string.substr(1, current_string.size() - 2);
      }

      current_state = _keywordTable[current_state][current_keyword];

      sqlTokenQueue.pop();
    }

    if (current_state == -1 && !is_success(_keywordTable, current_state)) {
      mmap.clear();
      return false;
    }

    return true;
  }

  bool insertQueueToMMap(Queue<SQLToken> &sqlTokenQueue,
                         MMap<std::string, std::string> &mmap) {
    int current_state = 6;
    mmap["command"] += std::string("insert");
    sqlTokenQueue.pop();

    while (!sqlTokenQueue.empty() && current_state != -1) {

      std::string current_string = sqlTokenQueue.front().token_str();
      SQL_KEYWORDS current_keyword = sqlTokenQueue.front().type();

      if (current_state == 7 && current_keyword == SQL_WORD) {
        mmap["table_name"] += current_string;
      }

      if ((current_state == 9 || current_state == 11) &&
          (current_keyword == SQL_WORD || current_keyword == SQL_NUMBER)) {
        mmap["values"] += current_string;
      }

      if ((current_state == 9 || current_state == 11) &&
          current_keyword == SQL_STRING) {
        mmap["values"] += current_string.substr(1, current_string.size() - 2);
      }

      current_state = _keywordTable[current_state][current_keyword];

      sqlTokenQueue.pop();
    }

    if (current_state == -1 && !is_success(_keywordTable, current_state)) {
      mmap.clear();
      return false;
    }

    return true;
  }

private:
  // command string
  string _str;
  static int _keywordTable[MAX_ROWS][MAX_COLUMNS];

  // =====================================
  // TODO: Add more methods here if needed
  // =====================================
};

int Parser::_keywordTable[MAX_ROWS][MAX_COLUMNS] = {0};

#endif // PARSE_H