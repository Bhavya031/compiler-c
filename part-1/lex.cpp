#include <cctype>
#include <cstddef>
#include <iostream>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

enum TokenType {

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
  }

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
  }

  void skipComment() {
    if (curChar == '#') {
      while (curChar != '\n' && curChar != '\0') {
        nextChar();
      }
    }
  }

  Token getToken() {
    skipWhitespace();
    skipComment();

    Token token;
    std::string tokenText(1, curChar);

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
      token = Token("", TokenType::EOF_);
    } else if (curChar == '=') {
      if (peek() == '=') {
        nextChar();
        token = Token("==", TokenType::EQEQ);
      } else {
        token = Token("=", TokenType::EQ);
      }
    } else if (curChar == '!') {
      if (peek() == '=') {
        nextChar();
        token = Token("!=", TokenType::NOTEQ);
      } else {
        throw std::runtime_error("Expected !=, got !" + std::string(1, peek()));
      }
    } else if (curChar == '<') {
      if (peek() == '=') {
        nextChar();
        token = Token("<=", TokenType::LTEQ);
      } else {
        token = Token("<", TokenType::LT);
      }
    } else if (curChar == '>') {
      if (peek() == '=') {
        nextChar();
        token = Token(">=", TokenType::GTEQ);
      } else {
        token = Token(">", TokenType::GT);
      }
    } else if (isdigit(curChar)) {
      return readNumber();
    } else if (isalpha(curChar)) {
      return readIdentifier();
    } else {
      throw std::runtime_error("Unknown token: " + std::string(1, curChar));
    }

    nextChar();
    return token;
  }

private:
  Token readNumber() {
    std::string tokenText;
    while (isdigit(curChar) || curChar == '.') {
      tokenText += curChar;
      nextChar();
    }
    return Token(tokenText, TokenType::NUMBER);
  }

  Token readIdentifier() {
    std::string tokenText;
    while (isalnum(curChar)) {
      tokenText += curChar;
      nextChar();
    }

    Token tokenInstance;
    std::optional<TokenType> keyword = tokenInstance.checkIfKeyword(tokenText);
    if (keyword == std::nullopt) {
      return Token(tokenText, TokenType::IDENT);
    } else {
      return Token(tokenText, *keyword);
    }
  }
};
int main() {
    std::vector<std::string> testStrings = {
        // Basic Assignments
        "i=1",
        "let x = 10",
        "x=y",
        "result = a + b - c",

        // Arithmetic Operations
        "x = (a + b) * c / d - e",
        "let total = sum1 + sum2 + sum3",
        "x = y * (z + w)",
        "let area = 3.14 * radius * radius",
        "x = x + 1",

        // Control Flow Statements
        "if x == 10 then print x endif",
        "while i < 10 repeat i = i + 1 endwhile",
        "if x != y then print \"Not equal\" endif",
        "if a <= b then print \"Less or equal\" endif",
        "while count != 0 repeat count = count - 1 endwhile",
        "if x > y then z = x else z = y endif",

        // Input and Output
        "input name",
        "print \"Hello, World!\"",
        "print \"Value: \" + x",
        "input \"Enter value:\", value",

        // Comments and Whitespace
        "# This is a comment\nx = x + 1",
        "   let x = 5   ",
        "\n\nx = y\n\n",
        "# Multiple comments\n# Another comment\nx = x - 1",

        // Labels and Goto
        "goto label1\nlabel1:",
        "label start",
        "goto end\nprint x\nend:",

        // Strings with Special Characters
        "x = \"String with spaces\"",
        "x = \"String with a #hash symbol\"",
        "x = \"String with a newline\\n in it\"",
        "x = \"He said, \\\"Hello!\\\"\"",

        // Numbers and Edge Cases
        "let decimal = 123.456",
        "let integer = 789",
        "x = 0",
        "x = .5",
        "x = 5.",

        // Invalid or Edge Inputs
        "123abc",
        "!@#$%^&*()",
        "x = y >> 1",
        "x = y << 1",
        "x = y & z",
        "x = y | z",
        "x = y ^ z",
        "x = ~y",
        "/* Comment */ x = 1",
        "x = 0x1A",
        "x = 0755",
        "x = 'Single quotes?'",
        "x = \"Unclosed string",
        "if (a == b) then print \"Equal\" endif",
        "while (i < 10) repeat i = i + 1 endwhile",

        // Empty and Whitespace Strings
        "",
        "   ",
        "\n",
        "\t\t",

        // Combined Statements
        "let x = 10\nif x > 5 then print x endif",
        "while i <= 10 repeat\n  print i\n  i = i + 1\nendwhile",
        "if x == y then\n  if y == z then\n    print \"All equal\"\n  endif\nendif",
        "let str = \"Hello\" + \" \" + \"World!\"",

        // Error Handling
        "x = y / 0",
        "unknown_token"
    };

    for (const auto& input : testStrings) {
        std::cout << "Tokenizing: \"" << input << "\"\n";
        LEX lexer(input);
        Token token;

        try {
            do {
                token = lexer.getToken();
                std::string tokenTypeStr = tokenTypeToString(token.getTokenKind());
                std::cout << "Token: \"" << token.getTokenText()
                          << "\" Type: " << tokenTypeStr << std::endl;
            } while (token.getTokenKind() != TokenType::EOF_);
        } catch (const std::exception& e) {
            std::cerr << "Error tokenizing input: " << e.what() << std::endl;
        }

        std::cout << "-----------------------------------\n";
    }

    return 0;
}
