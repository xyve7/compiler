#include <codegen.hpp>
#include <format>
#include <parser.hpp>
#include <unordered_map>
#include <variant>
namespace Codegen {

size_t align(size_t value, size_t alignment) {
    return (value - 1 + alignment) & -alignment;
}

std::unordered_map<std::string, IntegerSize *> integer_types = {
    {"i8",    new IntegerSize(true,  1)},
    {"i16",   new IntegerSize(true,  2)},
    {"i32",   new IntegerSize(true,  4)},
    {"i64",   new IntegerSize(true,  8)},
    {"isize", new IntegerSize(true,  8)},

    {"u8",    new IntegerSize(false, 1)},
    {"u16",   new IntegerSize(false, 2)},
    {"u32",   new IntegerSize(false, 4)},
    {"u64",   new IntegerSize(false, 8)},
    {"usize", new IntegerSize(false, 8)},
};
std::unordered_map<size_t, std::string> size_directives = {
    {1, "BYTE PTR" },
    {2, "WORD PTR" },
    {4, "DWORD PTR"},
    {8, "QWORD PTR"},
};

std::unordered_map<size_t, std::string> return_registers = {
    {1, "al" },
    {2, "ax" },
    {4, "eax"},
    {8, "rax"},
};

void Context::emit_function_declaration(FunctionDeclaration *declaration) {
    this->buffer << declaration->name->value << ":" << "\n";
    this->buffer << "push rbp" << "\n";
    this->buffer << "mov rbp, rsp" << "\n";
    emit_block(declaration->body);
    this->buffer << "pop rbp" << "\n";
    this->buffer << "ret" << "\n";
}
void Context::emit_variable_declaration(VariableDeclaration *declaration) {
    // The RHS value changes how the code is emitted
    auto type = integer_types[declaration->type->value->value];
    auto expression = declaration->rhs;
    if (auto i = std::get_if<Integer *>(&expression)) {
        auto directive = size_directives[type->size];
        auto off = align(this->stack_offset + type->size, type->alignment);
        auto rhs = (*i)->value->value;

        this->buffer << std::format("mov {} [rbp - {}], {}\n", directive, off, rhs);

        this->stack_offset = off;
    }
}
void Context::emit_return(Return *return_) {
    // The RHS value changes how the code is emitted
    auto expression = return_->rhs;
    if (auto i = std::get_if<Integer *>(&expression)) {
        auto rhs = (*i)->value->value;
        this->buffer << std::format("mov {}, {}\n", "rax", rhs);
    }
}
void Context::emit_statement(Statement statement) {
    if (auto fn = std::get_if<FunctionDeclaration *>(&statement)) {
        emit_function_declaration(*fn);
    } else if (auto vd = std::get_if<VariableDeclaration *>(&statement)) {
        emit_variable_declaration(*vd);
    } else if (auto r = std::get_if<Return *>(&statement)) {
        emit_return(*r);
    }
}
void Context::emit_block(Block *block) {
    for (auto s : block->statements) {
        emit_statement(s);
    }
}
void Context::emit() {
    this->buffer << ".intel_syntax noprefix" << "\n";
    this->buffer << ".text" << "\n";
    emit_block(this->root);
}

} // namespace Codegen
