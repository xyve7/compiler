#include <cassert>
#include <iostream>
#include <magic_enum/magic_enum.hpp>
#include <optional>
#include <parser.hpp>
#include <string>
#include <token.hpp>
#include <vector>

Statement parse_statement(std::vector<Token *>::iterator &it);
Expression parse_expression(std::vector<Token *>::iterator &it);
Expression parse_term(std::vector<Token *>::iterator &it);
Expression parse_factor(std::vector<Token *>::iterator &it);

bool match(std::vector<Token *>::iterator &it, const std::string &v) {
    auto id = **it;
    it++;
    return id == v;
}
bool match(std::vector<Token *>::iterator &it, TokenKind v) {
    auto id = **it;
    it++;
    return id == v;
}
bool peek(std::vector<Token *>::iterator &it, const std::string &v) {
    auto id = **it;
    return id == v;
}
bool peek(std::vector<Token *>::iterator &it, TokenKind v) {
    auto id = **it;
    return id == v;
}
Token *expect(std::vector<Token *>::iterator &it, const std::string &v) {
    auto id = *it;
    it++;
    if (*id != v) {
        std::cerr << "Expected " << v << ", got " << *id << " instead\n";
    }
    return id;
}
Token *expect(std::vector<Token *>::iterator &it, TokenKind v) {
    auto id = *it;
    it++;
    if (*id != v) {
        std::cerr << "Expected " << magic_enum::enum_name(v) << ", got " << *id << " instead\n";
    }
    return id;
}
Type *parse_type(std::vector<Token *>::iterator &it) {
    auto value = expect(it, TokenKind::TYPE);
    return new Type(value);
}
Expression parse_expression(std::vector<Token *>::iterator &it) {
    auto lhs = parse_term(it);
    while (peek(it, TokenKind::PLUS) || peek(it, TokenKind::MINUS)) {
        char op;
        if (match(it, TokenKind::PLUS)) {
            op = '+';
        } else {
            op = '-';
        }
        auto rhs = parse_term(it);
        lhs = new BinaryOp(lhs, op, rhs);
    }
    return lhs;
}
Expression parse_term(std::vector<Token *>::iterator &it) {
    auto lhs = parse_factor(it);
    while (peek(it, TokenKind::ASTERISK) || peek(it, TokenKind::SLASH)) {
        char op;
        if (match(it, TokenKind::ASTERISK)) {
            op = '*';
        } else {
            op = '/';
        }
        auto rhs = parse_factor(it);
        lhs = new BinaryOp(lhs, op, rhs);
    }
    return lhs;
}
Expression parse_factor(std::vector<Token *>::iterator &it) {
    if (peek(it, TokenKind::INTEGER)) {
        auto value = expect(it, TokenKind::INTEGER);
        return new Integer(value);
    } else if (peek(it, TokenKind::OPEN_PAREN)) {
        expect(it, TokenKind::OPEN_PAREN);
        auto expression = parse_expression(it);
        expect(it, TokenKind::CLOSE_PAREN);
        return expression;
    }
    __builtin_unreachable();
}
VariableDeclaration *parse_variable_declaration(std::vector<Token *>::iterator &it) {
    expect(it, "let");
    auto id = expect(it, TokenKind::ID);

    Type *type = nullptr;
    if (peek(it, TokenKind::COLON)) {
        expect(it, TokenKind::COLON);
        type = parse_type(it);
    }

    std::optional<Expression> expression = std::nullopt;
    if (peek(it, TokenKind::ASSIGN)) {
        expect(it, TokenKind::ASSIGN);
        expression = parse_expression(it);
    }
    expect(it, TokenKind::SEMICOLON);

    return new VariableDeclaration(id, type, expression);
}
Block *parse_block(std::vector<Token *>::iterator &it) {
    std::vector<Statement> statements;
    expect(it, OPEN_CURLY);
    while (!peek(it, CLOSE_CURLY)) {
        statements.push_back(parse_statement(it));
    }
    expect(it, CLOSE_CURLY);
    return new Block(std::move(statements));
}
FunctionDeclaration *parse_function_declaration(std::vector<Token *>::iterator &it) {
    expect(it, "fn");
    auto id = expect(it, TokenKind::ID);
    expect(it, TokenKind::OPEN_PAREN);
    std::vector<VariableDeclaration *> arguments;
    expect(it, TokenKind::CLOSE_PAREN);
    expect(it, TokenKind::COLON);
    auto type = parse_type(it);

    auto body = parse_block(it);

    return new FunctionDeclaration(id, std::move(arguments), type, body);
}
Return *parse_return(std::vector<Token *>::iterator &it) {
    expect(it, "return");

    std::optional<Expression> expression = std::nullopt;
    if (!peek(it, TokenKind::SEMICOLON)) {
        expression = parse_expression(it);
    }
    expect(it, TokenKind::SEMICOLON);

    return new Return(expression);
}
Statement parse_statement(std::vector<Token *>::iterator &it) {
    if (peek(it, "fn")) {
        return parse_function_declaration(it);
    } else if (peek(it, "let")) {
        return parse_variable_declaration(it);
    } else if (peek(it, "return")) {
        return parse_return(it);
    }
    __builtin_unreachable();
}
Block *parse(std::vector<Token *> &tokens) {
    auto it = tokens.begin();
    std::vector<Statement> statements;
    while (it < tokens.end()) {
        statements.push_back(parse_statement(it));
    }
    return new Block(std::move(statements));
}
