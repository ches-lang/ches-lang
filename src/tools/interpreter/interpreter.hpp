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
        int index = 0;
        std::stack<std::list<ByteSeq>> list;
        std::stack<std::stack<ByteSeq>> stack;
        std::stack<std::stack<ByteSeq>> opeStack;
        FuncList labelList;

        void printStackLog();

        void setLabelData();

        void runNextInst();

        std::string joinCode(ByteSeq src);
    };
}
