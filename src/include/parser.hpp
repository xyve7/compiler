#pragma once

#include <ostream>
#include <token.hpp>
#include <variant>
#include <vector>

class VariableDeclaration;
class FunctionDeclaration;
class Return;
class Integer;
class String;
std::ostream &operator<<(std::ostream &os, const FunctionDeclaration &v);

using Expression = std::variant<Integer *, String *>;
using Statement = std::variant<VariableDeclaration *, FunctionDeclaration *, Return *>;

class Type {
  public:
    Token *value;
    Type(Token *value) : value(value) {}
    friend std::ostream &operator<<(std::ostream &os, const Type &v) {
        os << v.value->value;
        return os;
    }
};

class Integer {
  public:
    Token *value;
    Integer(Token *value) : value(value) {}
    friend std::ostream &operator<<(std::ostream &os, const Integer &v) {
        os << v.value->value;
        return os;
    }
};

class String {
  public:
    Token *value;
    String(Token *value) : value(value) {}
    friend std::ostream &operator<<(std::ostream &os, const String &v) {
        os << v.value->value;
        return os;
    }
};

class VariableDeclaration {
  public:
    Token *name;
    Type *type;
    Expression rhs;
    VariableDeclaration(Token *name, Type *type, Expression rhs) : name(name), type(type), rhs(rhs) {}
    friend std::ostream &operator<<(std::ostream &os, const VariableDeclaration &v) {
        os << "let " << v.name->value << " = ";
        if (auto i = std::get_if<Integer *>(&v.rhs)) {
            os << **i;
        }
        os << ";";
        return os;
    }
};
class Return {
  public:
    Expression rhs;
    Return(Expression rhs) : rhs(rhs) {}
    friend std::ostream &operator<<(std::ostream &os, const Return &v) {
        os << "return ";
        if (auto i = std::get_if<Integer *>(&v.rhs)) {
            os << **i;
        }
        os << ";";
        return os;
    }
};
class Block {
  public:
    std::vector<Statement> statements;
    Block(std::vector<Statement> &&statements) : statements(statements) {}
    friend std::ostream &operator<<(std::ostream &os, const Block &v) {
        for (auto s : v.statements) {
            if (auto d = std::get_if<VariableDeclaration *>(&s)) {
                os << **d << "\n";
            } else if (auto d = std::get_if<FunctionDeclaration *>(&s)) {
                os << **d << "\n";
            }
        }
        return os;
    }
};

class FunctionDeclaration {
  public:
    Token *name;
    std::vector<VariableDeclaration *> arguments;
    Type *type;
    Block *body;
    FunctionDeclaration(Token *name, std::vector<VariableDeclaration *> &&arguments, Type *type, Block *body) : name(name), arguments(arguments), type(type), body(body) {}
    friend std::ostream &operator<<(std::ostream &os, const FunctionDeclaration &v) {
        os << "fn " << v.name->value << "() {\n"
           << *v.body << "}\n";
        return os;
    }
};

Block *parse(std::vector<Token *> &tokens);
