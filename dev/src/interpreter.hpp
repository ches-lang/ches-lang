#pragma once

#undef LN
#undef TK
#undef CH
#define LN(i)       (lines.at(i))
#define TK(i, j)    (lines.at(i).at(j))
#define CH(i, j, k) (lines.at(i).at(j).at(k))



class Interpreter {

public:

    Options options;
    Bytecode source;
    ByteSeq header;
    ByteSeq body;
    HeaderInfo headerInfo;
    InstList instList;

    Interpreter(Options options, Bytecode source);

    void run();

private:

    std::vector<std::stack<void*>> stacks;
    std::vector<FuncData> funcData;

    void runProgram();

    void setFuncData();

    void runInst(Instruction instruction);

    std::string joinCode(ByteSeq src);
};
