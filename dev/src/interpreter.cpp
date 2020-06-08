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
    //for(Byte c : src.source) std::cout << std::hex << (int)c << " "; std::cout << std::endl << std::endl;

    try {
        setFuncData(src);
        stacks.push_back(std::stack<void*>());
        runInst({ { IT_Jump }, FuncData::findByName(funcdata, { 0x6D, 0x61, 0x69, 0x6E }).id });
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void Interpreter::setFuncData(Bytecode src) {
    ByteSeq byteSeqSrc = src.source;

    ByteSeq header, body;
    std::copy(byteSeqSrc.begin(), byteSeqSrc.begin() + HEADER_LEN, std::back_inserter(header));
    std::copy(byteSeqSrc.begin() + HEADER_LEN, byteSeqSrc.end(), std::back_inserter(body));

    this->headerInfo = HeaderInfo(header);
    LineSeq lines = Bytecode(body).divide();

    std::cout<<std::hex;

    for(auto a : lines) {
        for(auto b : a) {
            for(auto c : b) {
                std::cout << (int)c << " ";
            } std::cout << "| ";
        } std::cout << std::endl;
    }

    if(this->headerInfo.magicNum != MAGIC_NUMBER)
        Console::log(LogType_Error, "8732", { { "Path", options.get("-i") } }, true);

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

    // 関数一覧を出力
    std::cout << std::endl; for(FuncData fd : funcdata) { std::cout << "funclist: "; for(Byte name : fd.name) { std::cout << name; } std::cout << std::endl; }
}

void Interpreter::runInst(TokenSeq inst) {
    //for(auto a : inst) for(auto b : a) std::cout << std::to_string(b) << " "; } std::cout << "| "; } std::cout << std::endl;

    try {
        switch(inst.at(0).at(0)) {
            case IT_Jump: {//
                //std::cout << "Called: " << joinCode(inst.at(1)) << std::endl;
                for(TokenSeq tks : FuncData::findById(funcdata, inst.at(1)).source)
                runInst(tks);
            } break;

            case IT_LSPush: {
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

std::string Interpreter::joinCode(ByteSeq src) {
    std::string res;
    for(Byte uc : src)
        res += std::to_string((int)uc) + " ";
    res.pop_back();
    return res;
}
