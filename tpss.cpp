#include <iostream>
#include <string>
#include <optional>

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

// Converts TokenType to string representation
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::EOF_: return "EOF";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::IDENT: return "IDENT";
        case TokenType::STRING: return "STRING";
        case TokenType::LABEL: return "LABEL";
        case TokenType::GOTO: return "GOTO";
        case TokenType::PRINT: return "PRINT";
        case TokenType::INPUT: return "INPUT";
        case TokenType::LET: return "LET";
        case TokenType::IF: return "IF";
        case TokenType::THEN: return "THEN";
        case TokenType::ENDIF: return "ENDIF";
        case TokenType::WHILE: return "WHILE";
        case TokenType::REPEAT: return "REPEAT";
        case TokenType::ENDWHILE: return "ENDWHILE";
        case TokenType::EQ: return "EQ";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::ASTERISK: return "ASTERISK";
        case TokenType::SLASH: return "SLASH";
        case TokenType::EQEQ: return "EQEQ";
        case TokenType::NOTEQ: return "NOTEQ";
        case TokenType::LT: return "LT";
        case TokenType::LTEQ: return "LTEQ";
        case TokenType::GT: return "GT";
        case TokenType::GTEQ: return "GTEQ";
        default: return "";
    }
}

std::optional<TokenType> checkIfKeyword(const std::string& tokenText) {
    static const TokenType allTokenTypes[] = {
        TokenType::EOF_, TokenType::NEWLINE, TokenType::NUMBER, TokenType::IDENT, TokenType::STRING,
        TokenType::LABEL, TokenType::GOTO, TokenType::PRINT, TokenType::INPUT, TokenType::LET,
        TokenType::IF, TokenType::THEN, TokenType::ENDIF, TokenType::WHILE, TokenType::REPEAT,
        TokenType::ENDWHILE, TokenType::EQ, TokenType::PLUS, TokenType::MINUS, TokenType::ASTERISK,
        TokenType::SLASH, TokenType::EQEQ, TokenType::NOTEQ, TokenType::LT, TokenType::LTEQ,
        TokenType::GT, TokenType::GTEQ
    };

    for (const auto& kind : allTokenTypes) {
        int value = static_cast<int>(kind);
        if (tokenTypeToString(kind) == tokenText && value >= 100 && value < 200) {
            return kind;
        }
    }
    return std::nullopt;
}

int main() {
    std::string testToken = "ENDWHILE"; // Test token to check if it's a keyword

    // Test checkIfKeyword function
    auto result = checkIfKeyword(testToken);

    if (result.has_value()) {
        std::cout << "Token '" << testToken << "' is a keyword: " << tokenTypeToString(result.value()) << std::endl;
    } else {
        std::cout << "Token '" << testToken << "' is not a keyword." << std::endl;
    }

    return 0;
}

