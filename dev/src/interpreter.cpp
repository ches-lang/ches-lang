#pragma once

#include "interpreter.hpp"



Interpreter::Interpreter(Options options, ByteSeq source) {
    try {
        options = options;

        this->source = source;
        ByteSeq byteSeqSrc = this->source;

        // ヘッダとボディを取得
        this->header = byteSeqSrc.copy(0, HEADER_LEN - 1);
        this->body = byteSeqSrc.copy(HEADER_LEN, -1);

        this->headerInfo = HeaderInfo(this->header);
        LineSeq lines = ByteSeq(this->body).toLineSeq();

        // 命令リストを取得
        for(ByteSeq bytes : lines)
            this->instList.push_back(Instruction(bytes));

        for(auto ln : lines) {
            std::cout << ByteSeq(ln).toHexString(" ") << std::endl;
        } std::cout << std::endl;

        for(Instruction inst : this->instList)
            std::cout << inst.toText() << std::endl;

        if(this->headerInfo.magicNum != MAGIC_NUMBER)
            Console::log(LogType_Error, "8732", { { "Path", options["-i"] } }, true);
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void Interpreter::run() {
    runProgram();
    //バイトコードのチェックも忘れず
}

void Interpreter::runProgram() {
    try {
        setFuncData();
        //this->stackList->push_back(Stack());
        //runInst({ { IT_Jump }, FuncData::findByName(this->funcData, { 0x6D, 0x61, 0x69, 0x6E }).id });
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void Interpreter::setFuncData() {
    try {
        for(int i = 0; i < this->instList.size(); i++) {
            Instruction inst = this->instList.at(i);

            if(inst.opcode == IT_Label) {
                // ラベルの終了インデックスと内容を取得
                InstList labelCont;
                int labelEndIndex = i + 1;

                while(labelEndIndex < this->instList.size()) {
                    Instruction instLine = this->instList.at(labelEndIndex);

                    if(instLine.opcode != IT_Label) {
                        labelCont.push_back(instLine);
                        labelEndIndex++;
                    } else {
                        break;
                    }
                }

                // ラベルリストにデータを追加
                Function label = Function(inst.operand["id"], inst.operand["name"], i + 1, labelEndIndex);
                label.instList = labelCont;
                std::cout<<"this->funcList->size()"<<std::endl;
                std::cout<<this->funcList->size()<<std::endl;
                //this->funcList->push_back(label);

                i = labelEndIndex + 1;
            } else {
                // ラベル外の処理
            }
        }

        // 関数一覧を出力
        /*std::cout << std::endl; for(Function func : *this->funcList) { std::cout << "funclist: "; for(Byte name : func.name) { std::cout << name; } std::cout << std::endl; }

        std::cout << std::endl;

        for(Function func : *this->funcList) {
            std::cout << "funclist: ";

            for(Byte name : func.name)
                std::cout << name;
            
            std::cout << std::endl;
        }*/

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void Interpreter::runInst(Instruction instruction) {
    //for(auto a : inst) for(auto b : a) std::cout << std::to_string(b) << " "; } std::cout << "| "; } std::cout << std::endl;

    try {
        switch(instruction.opcode) {
            case IT_Unknown: {std::cout<<"Unknown"<<std::endl;
            } break;

            case IT_Jump: {std::cout<<"Jump"<<std::endl;//
                //std::cout << "Called: " << joinCode(inst.at(1)) << std::endl;
                for(Instruction inst : this->funcList->findById(instruction.operand["id"]).instList)
                    runInst(inst);
            } break;

            case IT_LSPush: {std::cout<<"LSPush"<<std::endl;
                //std::cout << "Push: " << (int)inst.at(1).at(0) << std::endl;
                //stacks.at(0).push((void*)&inst.at(1));
                this->stackList->begin()->push((void*)&instruction.operand["value"]);
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
