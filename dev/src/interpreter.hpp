#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include "bytecode.cpp"
#include "console.cpp"
#include "filemanager.cpp"

#undef LN
#undef TK
#undef CH
#define LN(i)       (lines.at(i))
#define TK(i, j)    (lines.at(i).at(j))
#define CH(i, j, k) (lines.at(i).at(j).at(k))



class Interpreter {

public:

    Options options;

    Interpreter(Options opt);

    void run();

private:

    std::vector<std::stack<void*>> stacks;
    std::vector<FuncData> funcdata;

    void runProgram(Bytecode src);

    void setFuncData(Bytecode src);

    void runInst(TokenSeq inst);

    std::string joinCode(ByteSeq src);
};
