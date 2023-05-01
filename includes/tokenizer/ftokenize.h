#ifndef FTOKENIZE_H
#define FTOKENIZE_H

#include <fstream>
#include <iostream>

#include "constants.h"
#include "stokenize.h"
#include "token.h"

class FTokenizer {
public:
  // Maximum number of characters in a block
  const int MAX_BLOCK = MAX_BUFFER;

  ~FTokenizer() { _f.close(); }

  // Constructor
  FTokenizer(char *fname) {
    _pos = 0;
    _blockPos = 0;
    _f.open(fname);

    _more = true;

    _f.seekg(0, _f.end);
    _fileLength = _f.tellg();
    _f.seekg(0, _f.beg);

    get_new_block();
  }

  // Extract one token (very similar to the way cin >> works)
  SToken next_token() {

    if (_pos >= _fileLength) {
      _more = false;
      return SToken();
    }

    if (_blockPos >= MAX_BUFFER) {
      _blockPos = 0;
      get_new_block();
    }

    SToken temp;

    _stk >> temp;

    if (temp.type() == TOKEN_UNKNOWN) {
      ++_blockPos;
      ++_pos;
    } else {
      _pos += temp.token_str().size();
      _blockPos += temp.token_str().size();
    }

    return temp;
  }

  bool more() { return _more; } // returns the current value of _more

  int pos() { return _pos; } // returns the value of _pos

  int block_pos() { return _blockPos; } // returns the value of _blockPos

  friend FTokenizer &operator>>(FTokenizer &f, SToken &t) {
    t = f.next_token();

    return f;
  }

private:
  // gets the new block from the file
  bool get_new_block() {

    char b[MAX_BUFFER + 1];

    _f.seekg(_pos);

    _f.read(b, MAX_BUFFER);

    b[MAX_BUFFER] = '\0';

    _stk.set_string(b);

    std::cout << std::endl
              << "----- New Block ---------------------[" << strlen(b)
              << "] bytes" << std::endl;

    return true;
  }

  int _fileLength;
  std::ifstream _f; // file being tokenized
  STokenizer _stk;  // STokenizer object to tokenize current block
  int _pos;         // current position in the file
  int _blockPos;    // current position in the current block
  bool _more;       // false if last token of the last block
                    //   has been processed and now we are at
                    //   the end of the last block.
};

#endif // FTOKENIZE_H