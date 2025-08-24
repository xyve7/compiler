#include <codegen.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <parser.hpp>
#include <sstream>
#include <token.hpp>
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Insufficent number of arguments!\n";
        return EXIT_FAILURE;
    }
    // Read in the source code
    std::ifstream in(argv[1]);
    std::stringstream ss;
    ss << in.rdbuf();
    std::string source = ss.str();

    auto tokens = tokenize(source);
    auto root = parse(tokens);

    Codegen::Context codegen(root);
    codegen.emit();
    std::cout << codegen.assembly() << "\n";
}
