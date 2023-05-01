#ifndef TABLE_H
#define TABLE_H

#include <cassert>  // assert
#include <cstring>  // strcmp
#include <fstream>  // fstream
#include <iomanip>  // setw
#include <iostream> // cout, endl
#include <string>   // string
#include <vector>   // vector

#include "../binary_files/file_record.h"
#include "../binary_files/utilities.h"

#include "../linked_list/queue/MyQueue.h"

#include "../map/map.h"
#include "../map/mmap.h"

#include "../rpn/rpn.h"
#include "../shunting_yard/shunting_yard.h"

#include "../token/leftparen.h"
#include "../token/logical.h"
#include "../token/relational.h"
#include "../token/rightparen.h"
#include "../token/token.h"
#include "../token/tokenstr.h"
#include "../token/universal.h"

//======================================
// TODO: include more lib here as needed
//======================================

class Table {
public:
  // TYPEDEFS
  typedef vector<string> vector_str;
  typedef vector<long> vector_long;
  typedef Map<string, long> map_sl;
  typedef Map<string, string> map_ss;
  typedef MMap<string, long> mmap_sl;
  typedef MMap<string, string> mmap_ss;

  // CONSTRUCTORS
  Table(const string &title = string(),
        const vector<string> &fields = vector<string>()) {
    if (fields.size() > 0) {
      create_table(title, fields);

    } else {

      vector<std::string> temp(5000, "");

      _title = title;
      _n_records = 0;

      fstream f;
      FileRecord r(temp);

      open_fileRW(f, (title + "_fields.txt").c_str());

      r.read(f, _n_records);

      int field_count = 0;

      for (int i = 0; i < 5000; ++i) {
        if (strlen(r.get_records()[i]) != 0) {
          _fields.push_back(r.get_records()[i]);
          ++field_count;
        }
      }

      f.close();

      open_fileRW(f, (title + ".tbl").c_str());

      vector<std::string> temp2(field_count, "");

      FileRecord r2(temp2);
      _n_records = 0;

      long bytes = r2.read(f, _n_records);

      for (int i = 0; i < field_count; ++i) {
        MMap<std::string, long> temp;
        _columns.push_back(temp);
      }

      while (bytes > 0) {

        vector<std::string> row;

        for (int i = 0; i < r2.get_records().size(); ++i) {
          (_columns[i])[r2.get_records()[i]] += _n_records;
          row.push_back(std::string(r2.get_records()[i]));
        }

        _grid.push_back(row);

        ++_n_records;

        bytes = r2.read(f, _n_records);
      }

      f.close();
    }
  }

  // SQL: CREATE TABLE
  string create_table(const string &title, const vector<string> &fields) {
    _title = title;
    _fields = fields;
    _n_records = 0;

    for (int i = 0; i < fields.size(); ++i) {
      MMap<std::string, long> temp;
      _columns.push_back(temp);
    }

    fstream f;

    open_fileW(f, (title + "_fields.txt").c_str());

    FileRecord r(fields);

    r.write(f);

    f.close();

    open_fileW(f, (title + ".tbl").c_str());

    f.close();

    return "";
  }

  // SQL: INSERT INTO
  string insert_into(const vector<char *> &record) {
    fstream f;

    vector<std::string> row;

    for (int i = 0; i < record.size(); ++i) {
      row.push_back(std::string(record[i]));
    }

    _grid.push_back(row);

    FileRecord r(row);

    open_fileRW(f, (_title + ".tbl").c_str());

    _n_records = r.write(f) + 1;

    for (int i = 0; i < record.size(); ++i) {
      (_columns[i])[record[i]] += (_n_records - 1);
    }

    f.close();

    return "";
  }

  string insert_into(const vector<string> &row) {
    _grid.push_back(row);

    fstream f;
    FileRecord r(row);

    open_fileRW(f, (_title + ".tbl").c_str());

    _n_records = r.write(f) + 1;

    for (int i = 0; i < row.size(); ++i) {
      (_columns[i])[row[i]] += (_n_records - 1);
    }

    f.close();

    return "";
  }

  // SQL: SELECT
  Table select(const vector<string> &fields, const string &lhs,
               const string &op, const string &rhs) {

    Queue<Token *> post;

    post.push(new TokenStr(lhs));
    post.push(new TokenStr(rhs));
    post.push(new Relational(op));

    return select(fields, post);
  }

  Table select(const vector<string> &fields,
               const vector<string> &expression = vector<string>()) {
    Queue<Token *> infix;

    for (std::string str : expression) {
      infix.push(stringToToken(str));
    }

    ShuntingYard s(infix);

    Queue<Token *> post = s.shunting_yard();

    return select(fields, post);
  }

  Table select(const vector<string> &fields, const Queue<Token *> &expression) {
    if (expression.size() == 0) {
      Table selected_table(
          ("_selected_table_" + std::to_string(SELECTED_COUNT)), fields);

      SELECTED_COUNT++;

      vector<int> field_numbers;
      vector<long> temp;

      for (int i = 0; i < fields.size(); i++) {
        field_numbers.push_back(getFieldNumber(fields[i]));
      }

      for (int i = 0; i < _n_records; ++i) {
        temp.push_back(i);

        vector<string> row;

        for (int j : field_numbers) {

          row.push_back(_grid[i][j]);
        }

        selected_table.insert_into(row);
      }

      _selected = temp;

      return selected_table;
    }

    std::vector<MMap<std::string, long>> columns;

    for (std::string str : fields) {
      columns.push_back(getColumn(str));
    }

    RPN rpn(expression);

    vector<long> record_numbers = rpn(fields, columns);

    _selected = record_numbers;

    Table selected_table(("_selected_table_" + std::to_string(SELECTED_COUNT)),
                         fields);

    SELECTED_COUNT++;

    vector<int> field_numbers;

    for (int i = 0; i < fields.size(); i++) {
      field_numbers.push_back(getFieldNumber(fields[i]));
    }

    for (int i = 0; i < record_numbers.size(); ++i) {
      vector<string> row;

      for (int j : field_numbers) {

        row.push_back(_grid[record_numbers[i]][j]);
      }

      selected_table.insert_into(row);
    }

    return selected_table;
  }

  // Get all selected record numbers
  vector<long> select_recnos() const { return _selected; }

  // Print table
  friend ostream &operator<<(ostream &outs, const Table &t) {
    outs << "Table name: " << t._title << ", records: " << t._n_records
         << std::endl;

    outs << std::right << std::setw(25) << "record";

    for (int i = 0; i < t._fields.size(); ++i) {
      outs << std::right << std::setw(25) << t._fields[i];
    }

    outs << std::endl;

    for (int i = 0; i < t._n_records; ++i) {
      outs << std::right << std::setw(25) << i;
      for (int j = 0; j < t._grid[i].size(); ++j) {

        outs << std::right << std::setw(25) << (t._grid[i][j]);
      }

      outs << std::endl;
    }

    return outs;
  }

  // Get the title of the table
  string title() const { return _title; }

  // Get the fields of the table
  vector<string> get_fields() { return _fields; }

  // Get the number of records in the table
  long record_count() const { return _n_records; }

  //======================================
  // TODO: add more methods here as needed
  //======================================

  MMap<std::string, long> getColumn(std::string field_name) const {
    for (int i = 0; i < _fields.size(); ++i) {
      if (field_name == _fields[i]) {
        return _columns[i];
      }
    }

    return MMap<std::string, long>();
  }

  MMap<std::string, long> getColumn(int i) const { return _columns[i]; }

  Token *stringToToken(std::string str) {
    if (str == "(") {
      return new LeftParen();
    } else if (str == ")") {
      return new RightParen();
    } else if (str == "and" || str == "or" || str == "not") {
      return new Logical(str);
    } else if (str == "<" || str == ">" || str == "<=" || str == ">=" ||
               str == "=" || str == "!=") {
      return new Relational(str);
    } else {
      return new TokenStr(str);
    }
  }

  int getFieldNumber(std::string field) {
    for (int i = 0; i < _fields.size(); ++i) {
      if (_fields[i] == field) {
        return i;
      }
    }

    return -1;
  }

  friend std::ostream &operator<<(std::ostream &outs,
                                  const std::vector<long> rhs);

private:
  // title of the table
  string _title;

  // fields of the table
  vector<string> _fields;

  // number of records in the table
  long _n_records;

  // selected records' numbers
  vector<long> _selected;

  // table map
  vector<MMap<std::string, long>> _columns;

  // for << operator ease
  vector<vector<std::string>> _grid;

  //===========================================
  // TODO: add more data members here as needed
  //===========================================
};

std::ostream &operator<<(std::ostream &outs, const std::vector<long> rhs) {
  for (int i = 0; i < rhs.size(); ++i) {
    outs << rhs[i] << " ";
  }

  return outs;
}

#endif // TABLE_H