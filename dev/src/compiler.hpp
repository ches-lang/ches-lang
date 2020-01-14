#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "bytecode.cpp"
#include "command.cpp"
#include "console.cpp"
#include "filemanager.cpp"
#include "lexer.cpp"
#include "parser.cpp"



class Compiler {

public:

    Options options;

    Compiler();

    Compiler(Options opt);

    void compile();

    Bytecode getBytecode();
};
