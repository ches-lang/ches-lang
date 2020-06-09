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
        //runInst({ { IT_Jump }, FuncData::findByName(this->funcData, { 0x6D, 0x61, 0x69, 0x6E }).id });
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void Interpreter::setFuncData(Bytecode src) {
    ByteSeq byteSeqSrc = src.source;

    // ヘッダとボディを取得
    ByteSeq header, body;
    std::copy(byteSeqSrc.begin(), byteSeqSrc.begin() + HEADER_LEN, std::back_inserter(header));
    std::copy(byteSeqSrc.begin() + HEADER_LEN, byteSeqSrc.end(), std::back_inserter(body));

    this->headerInfo = HeaderInfo(header);
    TokenSeq lines = Bytecode(body).divide();

    // 命令リストを取得
    for(ByteSeq bytes : lines)
        this->instList.push_back(Instruction(bytes));

    std::cout<<std::hex;

    for(auto a : lines) {
        for(auto b : a) {
            std::cout << (int)b << " ";
        } std::cout << std::endl;
    }

    if(this->headerInfo.magicNum != MAGIC_NUMBER)
        Console::log(LogType_Error, "8732", { { "Path", options.get("-i") } }, true);

    for(int i = 0; i < instList.size(); i++) {
        Instruction inst = instList[i];

        if(inst.opcode == IT_Label) {
            // ラベルの終了インデックスと内容を取得
            InstList labelCont;
            int labelEndIndex = i + 1;

            while(labelEndIndex < instList.size()) {
                Instruction instLine = instList[labelEndIndex];

                if(instLine.opcode != IT_Label) {
                    labelCont.push_back(instLine);
                    labelEndIndex++;
                } else {
                    break;
                }
            }

            // ラベルリストにデータを追加
            FuncData labelData = FuncData(inst.operand["id"], inst.operand["name"], i + 1, labelEndIndex);
            labelData.instList = labelCont;
            this->funcData.push_back(labelData);

            i = labelEndIndex + 1;
        } else {
            // ラベル外の処理
        }
    }

    // 関数一覧を出力
    std::cout << std::endl; for(FuncData fd : this->funcData) { std::cout << "funclist: "; for(Byte name : fd.name) { std::cout << name; } std::cout << std::endl; }
}

void Interpreter::runInst(Instruction instruction) {
    //for(auto a : inst) for(auto b : a) std::cout << std::to_string(b) << " "; } std::cout << "| "; } std::cout << std::endl;

    try {
        switch(instruction.opcode) {
            case IT_Unknown: {std::cout<<"Unknown"<<std::endl;
            } break;

            case IT_Jump: {std::cout<<"Jump"<<std::endl;//
                //std::cout << "Called: " << joinCode(inst.at(1)) << std::endl;
                for(Instruction inst : FuncData::findById(this->funcData, instruction.operand["id"]).instList)
                    runInst(inst);
            } break;

            case IT_LSPush: {std::cout<<"LSPush"<<std::endl;
                //std::cout << "Push: " << (int)inst.at(1).at(0) << std::endl;
                //stacks.at(0).push((void*)&inst.at(1));
                stacks.at(0).push((void*)&instruction.operand["value"]);
            } break;

            default: {std::cout<<"Unknown_"<<std::endl;
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
