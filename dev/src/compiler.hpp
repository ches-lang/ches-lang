#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>



class Compiler {

public:

    Options options;

    Compiler();

    Compiler(Options opt);

    void compile();

    Bytecode getBytecode();
};
