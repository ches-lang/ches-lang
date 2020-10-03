#pragma once

#undef LN
#undef TK
#undef CH
#define LN(i)       (lines.at(i))
#define TK(i, j)    (lines.at(i).at(j))
#define CH(i, j, k) (lines.at(i).at(j).at(k))



class Interpreter {

public:

    ByteSeq source;
    ByteSeq header;
    ByteSeq body;
    HeaderInfo headerInfo;
    InstList instList;

    Interpreter(std::string filePath, ByteSeq source);

    void runProgram();

private:

    StackList stackList = {};
    FuncList labelList;

    void setLabelData();

    void runInst(Instruction instruction);

    std::string joinCode(ByteSeq src);
};
