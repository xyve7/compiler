#pragma once

#include <cstddef>
#include <magic_enum/magic_enum.hpp>
#include <ostream>
#include <string>
#include <vector>

enum TokenKind {
    ID,
    INTEGER,
    STRING,
    KEYWORD,
    TYPE,
    // Symbols
    PLUS,
    MINUS,
    SLASH,
    ASTERISK,
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_CURLY,
    CLOSE_CURLY,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    COMMA,
    DOT,
    COLON,
    SEMICOLON,
    ASSIGN,
};
class Token {
  public:
    std::string value;
    TokenKind kind;
    size_t col;
    size_t line;

    Token(std::string &&value, TokenKind kind, std::size_t col, size_t line) : value(value), kind(kind), col(col), line(line) {}
    bool operator==(const Token &rhs) const {
        return (this->value == rhs.value) && (this->kind == rhs.kind);
    }
    bool operator==(const std::string &rhs) const {
        return this->value == rhs;
    }
    bool operator==(TokenKind rhs) const {
        return this->kind == rhs;
    }
    friend std::ostream &operator<<(std::ostream &os, const Token &v) {
        os << magic_enum::enum_name(v.kind) << "('" << v.value << "') (" << v.col << ", " << v.line << ")";
        return os;
    }
};

std::vector<Token *> tokenize(const std::string &source);
