#pragma once

#include "interpreter.hpp"


ches::run::Interpreter::Interpreter(std::string filePath, ByteSeq source) {
    try {
        this->source = source;

        // ヘッダ長が足りない場合
        if(this->source.size() < HEADER_LEN)
            Console::log(LogType_Error, 5173, { { "Path", filePath } });

        // ヘッダとボディを取得
        this->header = this->source.copy(0, HEADER_LEN - 1);
        this->body = this->source.copy(HEADER_LEN, -1);

        this->headerInfo = HeaderInfo(this->header);
        LineSeq lines = ByteSeq(this->body).toLineSeq();

        // 命令リストを取得
        for(ByteSeq bytes : lines)
            this->instList.push_back(Instruction(bytes));

        // バイトコード&アセンブリコードのログを出力

        std::vector<std::string> rawLines;
        std::vector<std::string> asmLines;

        for(int i = 0; i < lines.size(); i++) {
            std::string index = std::to_string(i);

            while(index.length() < 3)
                index = "0" + index;

            rawLines.push_back(index + "| " + lines.at(i).toHexString(" "));
        }

        for(int i = 0; i < this->instList.size(); i++) {
            std::string index = std::to_string(i);

            while(index.length() < 3)
                index = "0" + index;

            asmLines.push_back(index + "| " + this->instList.at(i).toText());
        }

        Console::printDebugLog("raw bytecode", rawLines);
        Console::printDebugLog("assembly code", asmLines);

        // マジックナンバーをチェック
        if(this->headerInfo.magicNum != MAGIC_NUMBER)
            Console::log(LogType_Error, 8732, { { "Path", filePath } }, true);
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void ches::run::Interpreter::runProgram() {
    try {
        // ラベルデータを設定
        setLabelData();

        // ラベル一覧のログを出力

        std::vector<std::string> logLines;

        for(int i = 0; i < this->labelList.size(); i++)
            logLines.push_back("[" + std::to_string(i) + "] " + this->labelList.at(0).name.toHexString());

        Console::printDebugLog("label list", logLines);

        // エントリポイントの呼び出し
        ByteSeq bytes = { IT_Jump };
        bytes.push_back(this->labelList.findByName({ 0x6D, 0x61, 0x69, 0x6E }).id);
        runInst(Instruction(bytes));

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void ches::run::Interpreter::setLabelData() {
    try {
        for(int i = 0; i < this->instList.size(); i++) {
            Instruction inst = this->instList.at(i);

            if(inst.opcode == IT_Label) {
                // ラベルの終了インデックスと内容を取得
                InstList labelCont;
                int labelStartIndex = i++;

                // ラベルの終了位置を取得
                while(i < this->instList.size()) {
                    Instruction instLine = this->instList.at(i);

                    if(instLine.opcode != IT_Label) {
                        labelCont.push_back(instLine);
                        i++;
                    } else {
                        break;
                    }
                }

                // ラベルリストにデータを追加
                Function label = Function(inst.operand["id"], inst.operand["name"], labelStartIndex, --i);
                label.instList = labelCont;
                this->labelList.push_back(label);
            } else {
                // ラベル外の処理
            }
        }
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void ches::run::Interpreter::runInst(Instruction instruction) {
    //for(auto a : inst) for(auto b : a) std::cout << std::to_string(b) << " "; } std::cout << "| "; } std::cout << std::endl;

    try {
        switch(instruction.opcode) {
            case IT_Unknown: {std::cout<<"Unknown"<<std::endl;
            } break;

            case IT_Jump: {std::cout<<"Jump"<<std::endl;//
                //std::cout << "Called: " << joinCode(inst.at(1)) << std::endl;
                for(Instruction inst : this->labelList.findById(instruction.operand["id"]).instList)
                    runInst(inst);
            } break;

            case IT_LSPush: {std::cout<<"LSPush"<<std::endl;
                //std::cout << "Push: " << (int)inst.at(1).at(0) << std::endl;
                //stacks.at(0).push((void*)&inst.at(1));
                this->stackList.begin()->push((void*)&instruction.operand["value"]);
            } break;

            default: {std::cout<<"Unknown_"<<std::endl;
            } break;
        }
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

std::string ches::run::Interpreter::joinCode(ByteSeq src) {
    std::string res;
    for(Byte uc : src)
        res += std::to_string((int)uc) + " ";
    res.pop_back();
    return res;
}
