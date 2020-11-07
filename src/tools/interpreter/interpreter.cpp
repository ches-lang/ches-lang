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

            asmLines.push_back(index + "| " + InstConv::toString(this->instList.at(i)));
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

        for(Function label : this->labelList)
            logLines.push_back("[" + label.id.toHexString() + "] " + label.name.toString());

        Console::printDebugLog("label list", logLines);

        // エントリポイントの呼び出し

        this->list.push(std::list<ByteSeq>());
        this->stack.push(std::stack<ByteSeq>());

        ByteSeq lspush = { IT_LSPush };
        lspush.push_back(DST_32);
        ByteSeq id;

        for(Function label : this->labelList)
            if(label.name == ByteSeq("const"))
                id = label.id;

        lspush.push_back(id);
        runInst(Instruction(lspush));
        runInst(Instruction({ IT_Jump }));

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
                InstList labelInstList;
                int beginIndex = i++;

                // ラベルの終了位置を取得
                while(i < this->instList.size()) {
                    Instruction inst = this->instList.at(i);

                    if(inst.opcode != IT_Label) {
                        labelInstList.push_back(inst);
                        i++;
                    } else {
                        break;
                    }
                }

                int endIndex = i;

                ByteSeq id = inst.operand.at(0);
                ByteSeq name = inst.operand.at(1);

                this->labelList.push_back(Function(id, name, beginIndex, endIndex));
            } else {
                // ラベル外の処理
            }
        }
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void ches::run::Interpreter::runInst(Instruction instruction) {
    // for(auto a : instruction.operand) for(auto b : a) std::cout << std::to_string(b) << " "; std::cout << "| "; std::cout << std::endl;
    Console::writeln(instTypeMap.at(instruction.opcode));
    // Console::writeln(instruction.operand.size());

    try {
        switch(instruction.opcode) {
            case IT_Unknown: {
            } break;

            case IT_Jump: {
                ByteSeq id = this->stack.top().top();
                Function label = this->labelList.findById(id);

                for(int i = label.begin; i < label.end; i++)
                    runInst(this->instList.at(i));
            } break;

            case IT_LSPush: {
                this->stack.top().push(instruction.operand.at(1));
            } break;

            default: {
            } break;
        }
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}
