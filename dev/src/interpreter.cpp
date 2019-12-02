#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "bytecode.cpp"
#include "console.cpp"
#include "filemanager.cpp"

#define LN(i)       (divided.at(i))
#define TK(i, j)    (divided.at(i).at(j))
#define CH(i, j, k) (divided.at(i).at(j).source.at(k))



typedef struct FuncData_ {

    std::string name;
    int start;
    int end;

    FuncData_(std::string nm, int st, int ed) {
        name = nm;
        st = start;
        ed = end;
    }

} FuncData;



class Interpreter {

public:

    std::string filepath;

    Interpreter(std::string path) {
        filepath = path;
    }

    void run() {
        runInsts(FileManager::readBytecode(filepath));
        //バイトコードのチェックも忘れず
    }

private:

    std::vector<std::vector<Bytecode>> divided;
    std::vector<FuncData> funcdata;
    int index = -1;

    void runInsts(Bytecode src) {
        for(unsigned char c : src.source) std::cout << (int)c << " "; std::cout << std::endl << std::endl;

        try {

            divided = src.divide();

            if(TK(0, 0).source != std::vector<unsigned char> { 0x63, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x64, 0x5f, 0x63, 0x68, 0x65, 0x73 })
                error("magic number", true);

            while(index < divided.size()) {
                switch(CH(index, 0, 0)) {
                    case I_UNKNOWN:
                    break;

                    case I_GROUP:
                    std::cout << divided.at(index).size();
                    if(CH(index, 1, 0) == I_GROUP_CONST) {
                        //
                    } else {
                        funcdata.push_back(FuncData(divided.at(index).at(2).toString(), divided.at(index).at(0).toInt(), divided.at(index).at(1).toInt()));
                    }
                    break;

                    default:
                    //error
                    break;
                }
                index++;
            }

            for(FuncData fd : funcdata) std::cout << fd.name << std::endl; std::cout << std::endl;
            //ここの最後でmainを呼ぶ

        } catch(std::out_of_range ignored) {
            std::cout << "EXCEPTION" << std::endl;
        }
    }
};
