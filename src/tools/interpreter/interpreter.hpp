#pragma once

#define L_AT(lineIndex)                     (lines.at(lineIndex))
#define L_TOKEN_AT(lineIndex, tokenIndex)   (lines.at(lineIndex).at(tokenIndex))


namespace ches::run {
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
        std::stack<std::list<ByteSeq>> list;
        std::stack<std::stack<ByteSeq>> stack;
        FuncList labelList;

        void setLabelData();

        void runInst(Instruction instruction);

        std::string joinCode(ByteSeq src);
    };
}
