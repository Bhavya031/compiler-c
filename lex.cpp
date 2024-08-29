#include <iostream> // Use <iostream> for C++

class LEX {

private:
  std::string source;
  char curChar;
  int curPos;

public:
  LEX(const std::string &src) : source(src + '\n'), curChar('\0'), curPos(0) {
    nextChar();
  };
  void nextChar() {
    curPos++;
    if (curPos >= static_cast<int>(source.length())) {
      curChar = '\0';
    } else {
      curChar = source[curPos];
    }
  }
  char peek() {
    if (curPos + 1 >= static_cast<int>(source.length())) {
      return '\0';
    }
    return source[curPos + 1];
  }
};

int main() { return 0; }
