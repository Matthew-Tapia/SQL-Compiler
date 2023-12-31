#ifndef SQL_H
#define SQL_H

#include <cassert>  // assert
#include <fstream>  // ifstream
#include <iostream> // cout, endl

#include "../../includes/map/mmap.h"
#include "../../includes/parser/parser.h"
#include "../../includes/table/table.h"

using namespace std;

class SQL {
public:
  // Constructor
  SQL() {}

  // Process commands from a file
  SQL(char *&file) {}

  void batch(const string &cstr) {
    Table t;

    int counter = 0;
    vector<std::string> temp;

    fstream f;

    f.open(cstr.c_str(), ios::in);

    if (f.is_open()) {
      string str;

      while (getline(f, str)) {
        temp.push_back(str);
      }
    }

    for (string str : temp) {
      if (str.size() < 2 || str.substr(0, 2) == "//") {
        std::cout << str << endl;
      } else {
        cout << "[" << counter << "]"
             << " > " << str << endl;

        t = command(str);

        if (str.substr(0, 6) == "select") {

          std::cout << "records selected: " << t.select_recnos() << endl;
        }

        ++counter;
      }
    }

    f.close();
  }

  // Process a command and return the result table
  Table command(const string &cstr) {
    _parser.set_string(cstr);
    _command = _parser.parse_tree();

    if (_command.empty()) {
      return Table();
    }

    if (_command.get("command")[0] == "create") {
      _table = Table(_command.get("table_name")[0], _command.get("fields"));
    }

    if (_command.get("command")[0] == "insert") {
      _table = Table(_command.get("table_name")[0]);
      _table.insert_into(_command.get("values"));
    }

    if (_command.get("command")[0] == "select") {
      _table = Table(_command.get("table_name")[0]);

      if (_command.get("where").size() == 0) {

        std::cout << _table.select(_command.get("fields")) << endl;

      } else {

        std::cout << _table.select(_command.get("fields"),
                                   _command.get("condition"));
      }
    }

    return _table;
  }

  // Get all selected record numbers
  vector<long> select_recnos() { return _table.select_recnos(); }

  // Print command
  void print_lookup() { _command.print_lookup(); }

  //======================================
  // TODO: add more methods here as needed
  //======================================
private:
  Table _table;
  Parser _parser;
  MMap<string, string> _command;
  //======================================
  // TODO: add more methods here as needed
  //======================================
};

#endif // SQL_H