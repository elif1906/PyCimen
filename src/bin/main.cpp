#include <Python.h>
#include <numpy/arrayobject.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "../libs/lexer/lexer.hpp"
#include "../libs/parser/parser.hpp"
#include "../libs/ast/ast.hpp"
#include "../libs/interpreter/interpreter.hpp"
#include <string.h>

using namespace std;

inline void show_tokens(const vector<Token>& tokens) {
    for (const auto& token : tokens) {
        cout << token << '\n';
    }
    cout << flush;
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " [filename].pcl\n";
        return 1;
    }

    if (strcmp(argv[1], "--version") == 0) {
        cout << "version: 1.0.0" << std::endl;
        return 0;
    }

    const char* const filename = argv[1];
    ifstream inputFile(filename);

    if (!inputFile) {
        cerr << "Error: could not open file '" << filename << "'\n";
        return 1;
    }

    string source((istreambuf_iterator<char>(inputFile)), {});

    try {
        Lexer lexer(source);
        vector<Token> tokens = lexer.scanTokens();

        #ifdef DEBUG
            /* To define DEBUG, use `make DEBUG=1` when compiling */
            show_tokens(tokens);
        #endif

        Parser parser(tokens);
        ProgramNode* root = parser.parse();

        Interpreter interpreter;
        interpreter.interpret(root);

    } catch (const runtime_error& err) {
        cerr << err.what() << '\n';
        return EXIT_FAILURE;
    }

    // Py_Initialize();
    // import_array();

    // PyRun_SimpleString("import numpy");

   

    // Py_Finalize();

    return 0;
}