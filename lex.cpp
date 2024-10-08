#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>

enum class TokenType {

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
std::string tokenTypeToString(TokenType type) {
  switch (type) {
  case TokenType::EOF_:
    return "EOF";
  case TokenType::NEWLINE:
    return "NEWLINE";
  case TokenType::NUMBER:
    return "NUMBER";
  case TokenType::IDENT:
    return "IDENT";
  case TokenType::STRING:
    return "STRING";
  case TokenType::LABEL:
    return "LABEL";
  case TokenType::GOTO:
    return "GOTO";
  case TokenType::PRINT:
    return "PRINT";
  case TokenType::INPUT:
    return "INPUT";
  case TokenType::LET:
    return "LET";
  case TokenType::IF:
    return "IF";
  case TokenType::THEN:
    return "THEN";
  case TokenType::ENDIF:
    return "ENDIF";
  case TokenType::WHILE:
    return "WHILE";
  case TokenType::REPEAT:
    return "REPEAT";
  case TokenType::ENDWHILE:
    return "ENDWHILE";
  case TokenType::EQ:
    return "EQ";
  case TokenType::PLUS:
    return "PLUS";
  case TokenType::MINUS:
    return "MINUS";
  case TokenType::ASTERISK:
    return "ASTERISK";
  case TokenType::SLASH:
    return "SLASH";
  case TokenType::EQEQ:
    return "EQEQ";
  case TokenType::NOTEQ:
    return "NOTEQ";
  case TokenType::LT:
    return "LT";
  case TokenType::LTEQ:
    return "LTEQ";
  case TokenType::GT:
    return "GT";
  case TokenType::GTEQ:
    return "GTEQ";
  default:
    return "";
  }
}
class Token {
private:
  std::string tokenText;
  TokenType tokenKind;

public:
  Token(const std::string &text, TokenType kind)
      : tokenText(text), tokenKind(kind) {}
  Token() : tokenText(""), tokenKind(TokenType::EOF_) {}

  const std::string &getTokenText() const { return tokenText; }
  TokenType getTokenKind() const { return tokenKind; }
  std::optional<TokenType> checkIfKeyword(const std::string &tokenText) {
    static const TokenType allTokenTypes[] = {
        TokenType::EOF_,     TokenType::NEWLINE,  TokenType::NUMBER,
        TokenType::IDENT,    TokenType::STRING,   TokenType::LABEL,
        TokenType::GOTO,     TokenType::PRINT,    TokenType::INPUT,
        TokenType::LET,      TokenType::IF,       TokenType::THEN,
        TokenType::ENDIF,    TokenType::WHILE,    TokenType::REPEAT,
        TokenType::ENDWHILE, TokenType::EQ,       TokenType::PLUS,
        TokenType::MINUS,    TokenType::ASTERISK, TokenType::SLASH,
        TokenType::EQEQ,     TokenType::NOTEQ,    TokenType::LT,
        TokenType::LTEQ,     TokenType::GT,       TokenType::GTEQ};

    for (const auto &kind : allTokenTypes) {
      int value = static_cast<int>(kind);
      if (tokenTypeToString(kind) == tokenText && value >= 100 && value < 200) {
        return kind;
      }
    }
    return std::nullopt;
  }
};

class LEX {

private:
  std::string source;
  char curChar;
  int curPos;

public:
  LEX(const std::string &src) : source(src + '\n'), curChar('\0'), curPos(-1) {
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
  void skipWhitespace() {
    while (curChar == ' ' || curChar == '\t' || curChar == '\r') {
      nextChar();
    }
  };
  void skipComment() {
    if (curChar == '#') {
      while (curChar == '\n') {
        nextChar();
      };
    }
  }
  Token getToken() {
    skipWhitespace();
    skipComment();
    std::string tokenText(1, curChar);
    Token token;
    if (curChar == '+') {
      token = Token(tokenText, TokenType::PLUS);
    } else if (curChar == '-') {
      token = Token(tokenText, TokenType::MINUS);
    } else if (curChar == '*') {
      token = Token(tokenText, TokenType::ASTERISK);
    } else if (curChar == '/') {
      token = Token(tokenText, TokenType::SLASH);
    } else if (curChar == '\n') {
      token = Token(tokenText, TokenType::NEWLINE);
    } else if (curChar == '\0') {
      token = Token(tokenText, TokenType::EOF_);
    } else if (curChar == '=') {
      if (peek() == '=') {
        nextChar();
        token = Token(tokenText + curChar, TokenType::EQEQ);
      }
    } else if (curChar == '!') {
      if (peek() == '=') {
        nextChar();
        tokenText += curChar;
        token = Token(tokenText, TokenType::NOTEQ);
      } else {
        throw std::runtime_error("Expected !=, got !" + std::string(1, peek()));
      }
    } else if (curChar == '<') {
      if (peek() == '=') {
        nextChar();
        tokenText += curChar;
        token = Token(tokenText, TokenType::LTEQ);
      } else {
        token = Token(tokenText, TokenType::LT);
      }
    } else if (curChar == '>') {
      if (peek() == '=') {
        nextChar();
        tokenText += curChar;
        token = Token(tokenText, TokenType::GTEQ);
      } else {
        token = Token(tokenText, TokenType::GT);
      }
    } else if (isdigit(curChar)) {
      int startPos = curPos;
      while (isdigit(peek())) {
        nextChar();
        if (curChar == '.') {
          nextChar();
          if (!isdigit(curChar)) {
            throw std::runtime_error("Illegal character in number.");
          }
        }
        while (isdigit(peek())) {
          nextChar();
        }
        tokenText = source.substr(startPos, curPos - startPos);
        token = Token(tokenText, TokenType::NUMBER);
      }
    } else if (isalpha(curChar)) {
      int startPos = curPos;
      while (isalpha(peek())) {
        nextChar();
      }
      tokenText = source.substr(startPos, curPos - startPos + 1);
      Token tokenInstance;
      std::optional<TokenType> keyword =
          tokenInstance.checkIfKeyword(tokenText);
      if (keyword == std::nullopt) {
        token = Token(tokenText, TokenType::IDENT);
      } else {
        token = Token(tokenText, *keyword);
      }
    }
    nextChar();
    return token;
  }
};

int main() {
  // The string to be tokenized
  std::string input = "IF ELSEIF";

  std::cout << "Tokenizing: \"" << input << "\"" << std::endl;

  LEX lexer(input);
  Token token;

  do {
    token = lexer.getToken();
    std::string tokenTypeStr = tokenTypeToString(token.getTokenKind());
    std::cout << "Token: " << token.getTokenText() << " Type: " << tokenTypeStr
              << std::endl;
    lexer.nextChar();
  } while (token.getTokenKind() != TokenType::EOF_);

  return 0;
}
