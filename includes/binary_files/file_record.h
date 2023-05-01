#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <cstring>  // strncpy
#include <fstream>  // fstream
#include <iomanip>  // setw, right
#include <iostream> // cout, endl
#include <vector>   // vector

using namespace std;

class FileRecord {
public:
  // When construct a FileRecord, it's either empty or it contains a word
  FileRecord() { recno = -1; }

  FileRecord(string s) {
    char *temp = new char[MAX + 1];

    std::strcpy(temp, s.c_str());

    _records.push_back(temp);
    ++recno;
  }

  FileRecord(char str[]) {
    _records.push_back(str);
    ++recno;
  }

  FileRecord(vector<string> v) {

    for (int i = 0; i < v.size(); ++i) {
      char *temp = new char[MAX + 1];

      std::strcpy(temp, v[i].c_str());

      _records.push_back(temp);
    }

    ++recno;
  }

  // Returns the record number
  long write(fstream &outs) {

    // Write to the end of the file
    outs.seekg(0, outs.end);

    // Retrieves the current position of the file pointer
    long pos = outs.tellp();

    // Insert the size of record items into file

    for (int i = 0; i < _records.size(); ++i) {
      outs.write(_records[i], MAX);
    }

    // Return the record number
    return pos / (MAX * _records.size());
  }

  // Returns the number of bytes read = MAX, or zero if read passed the end of
  // file
  long read(fstream &ins, long recno) {

    // Calculate the position of the record in the file

    long pos = recno * (MAX * _records.size());

    // Set the file pointer to the pos starting from the beginning of the file
    ins.seekg(pos, ios_base::beg);

    for (int i = 0; i < _records.size(); ++i) {
      // Read the record from the file
      ins.read(_records[i], MAX);

      // Set the end of cstring with null
      // For example: _records[] => [zero'\0' trash ... trash]
      _records[i][MAX] = '\0';
    }

    // Return the number of bytes read
    return ins.gcount() * _records.size();
  }

  // Returns the record
  vector<char *> get_records() { return _records; }

  // Overload the << operator to print a FileRecord
  friend ostream &operator<<(ostream &outs, const FileRecord &r) {
    for (char *_record : r._records) {
      outs << setw(MAX / 4) << right << _record;
    }
    return outs;
  }

  // =====================================
  // TODO: Add more methods here if needed
  // =====================================

private:
  // The maximum size of the record
  static const int MAX = 100;
  // The record number
  int recno;
  // The record vector
  vector<char *> _records;
};

#endif