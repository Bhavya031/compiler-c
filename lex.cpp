#include <iostream> // Use <iostream> for C++

class LEX
{

private:
  std::string source;
  char curChar;
  int curPos;

public:
  LEX(const std::string &src) : source(src + '\n'), curChar('\0'), curPos(-1)
  {
    nextChar();
  };
  void nextChar()
  {
    curPos++;
    if (curPos >= static_cast<int>(source.length()))
    {
      curChar = '\0';
    }
    else
    {
      curChar = source[curPos];
    }
  }
  char peek()
  {
    if (curPos + 1 >= static_cast<int>(source.length()))
    {
      return '\0';
    }
    return source[curPos + 1];
  }
  char getCurrentChar() const { return curChar; }
};
enum class TokenType
{

  EOF_ = -1,
  NEWLINE = 0,
  NUMBER = 1,
  IDENT = 2,
  STRING = 3,
  // Keywords
  LABEL = 101,
  GOTO = 102,
  PRINT = 103,
  INPUT = 104,
  LET = 105,
  IF = 106,
  THEN = 107,
  ENDIF = 108,
  WHILE = 109,
  REPEAT = 110,
  ENDWHILE = 111,
  // Operators
  EQ = 201,
  PLUS = 202,
  MINUS = 203,
  ASTERISK = 204,
  SLASH = 205,
  EQEQ = 206,
  NOTEQ = 207,
  LT = 208,
  LTEQ = 209,
  GT = 210,
  GTEQ = 211

};
void printTokenType(TokenType token)
{
  if (token == TokenType::EQ)
  {
    std::cout << "Token is EQ (equals)" << std::endl;
  }
  else
  {
    std::cout << "Token is not EQ" << std::endl;
  }

  // Print the underlying integer value
  std::cout << "Token value: " << static_cast<int>(token) << std::endl;
}
int main()
{

  TokenType token = TokenType::EQ;
  printTokenType(token);

  token = TokenType::PLUS;
  printTokenType(token);
  std::cout << "done";
  return 0;
}
