#pragma once

#include <cstdint>
#include <parser.hpp>
#include <sstream>
namespace Codegen {
class IntegerSize {
  public:
    bool is_signed;
    size_t size;
    size_t alignment;
    IntegerSize(bool is_signed, size_t size) : is_signed(is_signed), size(size), alignment(size) {}
    IntegerSize(bool is_signed, size_t size, size_t alignment) : is_signed(is_signed), size(size), alignment(alignment) {}
};
class Context {
  public:
    std::stringstream buffer;
    Block *root;
    uint64_t stack_offset;

    Context(Block *root) : root(root), stack_offset(0) {}

    void emit();
    std::string assembly() {
        return buffer.str();
    }

  private:
    void emit_block(Block *block);
    void emit_statement(Statement statement);
    void emit_function_declaration(FunctionDeclaration *declaration);
    void emit_variable_declaration(VariableDeclaration *declaration);
    void emit_return(Return *return_);
};
} // namespace Codegen
