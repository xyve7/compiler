#include <cctype>
#include <string>
#include <token.hpp>
#include <unordered_set>

std::unordered_set<std::string> types = {
    "bool",
    "i8",
    "i16",
    "i32",
    "i64",
    "i128",
    "isize",
    "u8",
    "u16",
    "u32",
    "u64",
    "u128",
    "usize",
    "f32",
    "f64",
    "void"
};
std::unordered_set<std::string> keywords = {
    "break",
    "const",
    "continue",
    "default",
    "do",
    "else",
    "enum",
    "extern",
    "for",
    "if",
    "return",
    "static",
    "struct",
    "match",
    "type",
    "union",
    "volatile",
    "while",
    "atomic",
    "noreturn",
    "thread_local",
    "fn",
    "let",
    "import",
};

std::vector<Token *> tokenize(const std::string &source) {
    std::vector<Token *> tokens;
    size_t col = 0;
    size_t line = 0;
    auto it = source.begin();
    while (it < source.end()) {
        switch (*it) {
        case ' ':
        case '\t':
            col++;
            it++;
            break;
        case '\n':
            col = 0;
            line++;
            it++;
            break;
        case '+': {
            auto token = new Token(std::string("+"), TokenKind::PLUS, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case '-': {
            auto token = new Token(std::string("-"), TokenKind::MINUS, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case '/': {
            auto token = new Token(std::string("/"), TokenKind::SLASH, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case '*': {
            auto token = new Token(std::string("*"), TokenKind::ASTERISK, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case '(': {
            auto token = new Token(std::string("("), TokenKind::OPEN_PAREN, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case ')': {
            auto token = new Token(std::string(")"), TokenKind::CLOSE_PAREN, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case '{': {
            auto token = new Token(std::string("{"), TokenKind::OPEN_CURLY, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case '}': {
            auto token = new Token(std::string("}"), TokenKind::CLOSE_CURLY, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case '[': {
            auto token = new Token(std::string("["), TokenKind::OPEN_BRACKET, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case ']': {
            auto token = new Token(std::string("]"), TokenKind::CLOSE_BRACKET, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case ',': {
            auto token = new Token(std::string(","), TokenKind::COMMA, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case '.': {
            auto token = new Token(std::string("."), TokenKind::DOT, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case ':': {
            auto token = new Token(std::string(":"), TokenKind::COLON, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case ';': {
            auto token = new Token(std::string(";"), TokenKind::SEMICOLON, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case '=': {
            auto token = new Token(std::string("="), TokenKind::ASSIGN, col, line);
            tokens.push_back(token);
            col++;
            it++;
            break;
        }
        case '"': {
            auto token_start = it;
            size_t col_start = col;
            do {
                it++;
                col++;
            } while (*it != '"');
            it++;
            col++;

            std::string token_string(token_start, it);
            Token *token = new Token(std::move(token_string), TokenKind::STRING, col_start, line);
            tokens.push_back(token);
            break;
        }
        default: {
            if (std::isalpha(*it) || *it == '_') {
                auto token_start = it;
                size_t col_start = col;
                do {
                    it++;
                    col++;
                } while (std::isalnum(*it) || *it == '_');

                std::string token_string(token_start, it);
                Token *token = nullptr;
                if (types.count(token_string) > 0) {
                    token = new Token(std::move(token_string), TokenKind::TYPE, col_start, line);
                } else if (keywords.count(token_string) > 0) {
                    token = new Token(std::move(token_string), TokenKind::KEYWORD, col_start, line);
                } else {
                    token = new Token(std::move(token_string), TokenKind::ID, col_start, line);
                }
                tokens.push_back(token);
            } else if (std::isdigit(*it)) {
                auto token_start = it;
                size_t col_start = col;
                do {
                    it++;
                    col++;
                } while (std::isdigit(*it));
                auto token = new Token(std::string(token_start, it), TokenKind::INTEGER, col_start, line);
                tokens.push_back(token);
            }
            break;
        }
        }
    }
    return tokens;
}
