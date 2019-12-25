#pragma once

#include <iostream>
#include <vector>
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

    Interpreter(Options opt) {
        options = opt;
    }

    void run() {
        runProgram(FileManager::readBytecode(options.get("-i")));
        //バイトコードのチェックも忘れず
    }

private:

    std::vector<std::vector<std::vector<unsigned char>>> lines;
    std::vector<FuncData> funcdata;

    void runProgram(Bytecode src) {
        for(unsigned char c : src.source) std::cout << (int)c << " "; std::cout << std::endl << std::endl;

        try {

            lines = src.divide();

            if(TK(0, 0) != std::vector<unsigned char> { 0x63, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x64, 0x5f, 0x63, 0x68, 0x65, 0x73 })
                Console::error("cerr8732", "invalid file", { { "path", options.get("-i") }, { "cause", "magic number" } }, true);

            for(int i = 1; i < lines.size(); ) {
                if(CH(i, 0, 0) == I_GROUP) {
                    int funcend = -1;

                    for(int j = i + 1; j < lines.size(); j++) {
                        if(CH(j, 0, 0) == I_GROUP) {
                            funcend = j - 1;
                            break;
                        }
                    }

                    if(funcend == -1) {
                        funcdata.push_back(FuncData(TK(i, 1), TK(i, 2), i, lines.size() - 1));
                        i = lines.size() - 1;
                        break;
                    } else {
                        funcdata.push_back(FuncData(TK(i, 1), TK(i, 2), i, funcend));
                        i = funcend + 1;
                    }
                } else {
                    i++;
                }
            }

            for(FuncData fd : funcdata) std::cout << fd.name[0] << std::endl; std::cout << std::endl;

            runInst({ { I_CALL }, FuncData::find(funcdata, { 0x6D, 0x61, 0x69, 0x6E }).id });

        } catch(std::out_of_range ignored) {
            std::cout << "EXCEPTION" << std::endl;
        }
    }

    void runInst(std::vector<std::vector<unsigned char>> inst) {
        try {

            switch(inst.at(0).at(0)) {

            }

        } catch(std::out_of_range ignored) {
            std::cout << "EXCEPTION" << std::endl;
        }
    }
};
