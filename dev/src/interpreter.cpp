#pragma once

#include "interpreter.hpp"



Interpreter::Interpreter(Options opt) {
    options = opt;
}

void Interpreter::run() {
    runProgram(FileManager::readBytecode(options.get("-i")));
    //バイトコードのチェックも忘れず
}

void Interpreter::runProgram(Bytecode src) {
    //for(unsigned char c : src.source) std::cout << std::hex << (int)c << " "; std::cout << std::endl << std::endl;

    try {
        setFuncData(src);
        stacks.push_back(std::stack<void*>());
        runInst({ { IT_Jump }, FuncData::findByName(funcdata, { 0x6D, 0x61, 0x69, 0x6E }).id });
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void Interpreter::setFuncData(Bytecode src) {
    Lines lines = src.divide();

    for(auto a : lines) {
        for(auto b : a) {
            for(auto c : b) {
                std::cout << (int)c << " ";
            } std::cout << "| ";
        } std::cout << std::endl;
    }

    if(TK(0, 0) != Code { 0x63, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x64, 0x5f, 0x63, 0x68, 0x65, 0x73 })
        Console::error("cerr8732", "invalid magic number", { { "path", options.get("-i") } }, true);

    for(int i = 1; i < lines.size(); i++) {
        if(CH(i, 0, 0) == IT_Label) {
            int funcend = -1;

            for(int j = i + 1; j < lines.size(); j++) {
                if(CH(j, 0, 0) == IT_Label) {
                    funcend = j - 1;
                    break;
                }
            }

            if(funcend == -1) {
                funcdata.push_back(FuncData(TK(i, 1), TK(i, 2), i, lines.size() - 1));
                break;
            } else {
                funcdata.push_back(FuncData(TK(i, 1), TK(i, 2), i, funcend));
            }
        } else {
            if(funcdata.size() >= 1) {
                funcdata.at(funcdata.size() - 1).source.push_back(LN(i));
            }
        }
    }

    for(FuncData fd : funcdata) std::cout << fd.name[0] << std::endl; std::cout << std::endl;
}

void Interpreter::runInst(Tokens inst) {
    //for(auto a : inst) for(auto b : a) std::cout << std::to_string(b) << " "; } std::cout << "| "; } std::cout << std::endl;

    try {
        switch(inst.at(0).at(0)) {
            case IT_Jump: {//
                //std::cout << "Called: " << joinCode(inst.at(1)) << std::endl;
                for(Tokens tks : FuncData::findById(funcdata, inst.at(1)).source)
                runInst(tks);
            } break;

            case IT_LocalStackPush: {
                //std::cout << "Push: " << (int)inst.at(1).at(0) << std::endl;
                stacks.at(0).push((void*)&inst.at(1));
            } break;

            case IT_Unknown: {
                //std::cout << "Unknown" << std::endl;
            } break;
        }
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

std::string Interpreter::joinCode(Code src) {
    std::string res;
    for(unsigned char uc : src)
        res += std::to_string((int)uc) + " ";
    res.pop_back();
    return res;
}
