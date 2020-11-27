#pragma once

#define HEADER_LEN      (128)
#define MAGIC_NUM       (ByteVec { 0x4F, 0x52, 0x49, 0x4E, 0x43, 0x48, 0x41, 0x4E })

#undef  INST

#define CONSOLE_OUT(value)  do { Console::writeln("* print\n    " + Interpreter::toHexString(value)); this->printStackLog(); } while(0)
#define INST                (this->instList.at(this->index))
#define INST_OPCODE         (INST.at(0))
#define LOAD()              do { this->opeStack.top().push(this->stack.top().top()); Console::writeln("* load\n    " + std::to_string(this->opeStack.top().size())); this->printStackLog(); } while(0)
#define NEXT_INST_RUN()     do { this->runNextInst(); /* Console::writeln("* run: " + InstConv::toString(inst)); this->printStackLog(); */ } while(0)
#define STACK_POP()         do { this->stack.top().pop(); Console::writeln("* pop\n    " + std::to_string(this->stack.top().size())); this->printStackLog(); } while(0)
#define STACK_PUSH(value)   do { this->stack.top().push(value); Console::writeln("* push\n    " + std::to_string(this->stack.top().size()) + " " + Interpreter::toHexString(value)); this->printStackLog(); } while(0)
#define STACK_TOP           (this->stack.top().top())
#define STORE()             do { this->opeStack.top().pop(); Console::writeln("* store\n    " + std::to_string(this->opeStack.top().size())); this->printStackLog(); } while(0)

#define L_AT(lineIndex)                     (lines.at(lineIndex))
#define L_TOKEN_AT(lineIndex, tokenIndex)   (lines.at(lineIndex).at(tokenIndex))

typedef std::stack<ches::ByteVec>   Stack;

#include "interpreter.cpp"


ches::run::Label::Label() {}

ches::run::Label::Label(ByteVec id, ByteVec name, int beginIndex, int endIndex) {
    this->id = id;
    this->name = name;
    this->beginIndex = beginIndex;
    this->endIndex = endIndex;
}


ches::run::Interpreter::Interpreter(std::string filePath) {
    try {
        this->loadCompiledFile(filePath);

        // ヘッダ長をチェック
        if(this->header.size() != HEADER_LEN)
            Console::log(LogType_Error, 5173, { { "Path", filePath } });

        // ヘッダとボディを取得
        // this->header = this->source.copy(0, HEADER_LEN - 1);
        // this->body = this->source.copy(HEADER_LEN, -1);

        // this->headerInfo = HeaderInfo(this->header);
        // LineSeq lines = ByteSeq(this->body).toLineSeq();

        // 命令リストを取得
        // for(ByteSeq bytes : lines)
            // this->instList.push_back(Instruction(bytes));

        // バイトコード&アセンブリコードのログを出力

        std::vector<std::string> rawLines;
        std::vector<std::string> asmLines;

        for(int i = 0; i < this->body.size(); i++) {
            std::string index = std::to_string(i);

            while(index.length() < 3)
                index = "0" + index;

            rawLines.push_back(index + "| " + Interpreter::toHexString(this->instList.at(i), " "));
        }

        for(int i = 0; i < this->instList.size(); i++) {
            std::string index = std::to_string(i);

            while(index.length() < 3)
                index = "0" + index;

            // asmLines.push_back(index + "| " + InstConv::toString(this->instList.at(i)));
            asmLines.push_back(index + "| " + "[asm]");
        }

        Console::printDebugLog("raw bytecode", rawLines);
        Console::printDebugLog("assembly code", asmLines);

        // マジックナンバーをチェック
        if(this->magicNum != MAGIC_NUM)
            Console::log(LogType_Error, 8732, { { "Path", filePath } }, true);
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void ches::run::Interpreter::loadCompiledFile(std::string filePath) {
    try {
        ByteVec result;
        std::ifstream ifs(filePath);

        if(!ifs.is_open())
            Console::log(LogType_Error, 327, { { "Path", filePath } }, true);

        if(ifs.fail())
            Console::log(LogType_Error, 6845, { { "Path", filePath } }, true);

        Byte byte;
        int loadedSize = 0;

        do {
            ifs.read((char*)&byte, sizeof(char));
            (loadedSize < HEADER_LEN ? this->header : this->body).push_back(byte);
            loadedSize++;
        } while(!ifs.eof());

        // if(result.size() > 0)
        //     result.pop_back();

        int magicNumSize = MAGIC_NUM.size();
        std::copy(this->header.begin(), this->header.end() - this->header.size() + 8 + 1, std::back_inserter(this->magicNum));

        ifs.close();
    } catch(std::exception excep) {
        Console::log(LogType_Error, 6845, { { "Path", filePath } }, true);
    }
}

void ches::run::Interpreter::printStackLog() {
    try {
        Console::writeln();
        Console::writeln("stacksize");
        Console::writeln();
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

        for(auto [ id, label ] : this->labelList)
            logLines.push_back("[" + Interpreter::toHexString(id) + "] " + Interpreter::toHexString(label.name));///name

        Console::printDebugLog("label list", logLines);

        // スタックの初期化

        this->stack.push(Stack());
        this->opeStack.push(Stack());

        // // エントリポイントの呼び出し

        // Label entryLabel;

        // エントリポイントのラベルを取得
        // for(auto [ id, label ] : this->labelList)
        //     if(label.name == ByteVec { 0x63, 0x6F, 0x6E, 0x73, 0x74 })
        //         entryLabel = label;

        // this->index = entryLabel.beginIndex + 1;

        NEXT_INST_RUN();

        // ByteSeq lspush = { IT_LSPush };
        // lspush.push_back(DST_32);
        // Function label = this->labelList.findByName(ByteSeq("const"));
        // lspush.push_back(label.id);
        // INST_RUN(Instruction(lspush));
        // INST_RUN(Instruction({ IT_Jump }));
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void ches::run::Interpreter::setLabelData() {
    try {
        for(int i = 0; i < this->instList.size(); i++) {
            ByteVec inst = this->instList.at(i);

            if(inst.at(0) == IT_Block) {
                // ラベルの終了インデックスと内容を取得
                // InstList labelInstList;
                int beginIndex = i++;

                // ラベルの終了位置を取得
                // while(i < this->instList.size()) {
                //     Instruction inst = this->instList.at(i);

                //     if(inst.opcode != IT_Label) {
                //         labelInstList.push_back(inst);
                //         i++;
                //     } else {
                //         i--;
                //         break;
                //     }
                // }

                int endIndex = i;

                ByteVec id;
                std::copy(inst.begin() + 1, inst.end() - inst.size() + 18, std::back_inserter(id));;
                ByteVec name;
                std::copy(inst.begin() + 19, inst.end(), std::back_inserter(name));;

                this->labelList[id] = Label(id, name, beginIndex, endIndex);
            } else {
                // ラベル外の処理
            }
        }
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void ches::run::Interpreter::runNextInst() {
    try {
        Console::writeln(instTypeMap.at(INST_OPCODE));
        Console::writeln();

        if(this->index >= this->instList.size())
            return;

        switch(INST_OPCODE) {
            case IT_Unknown: {
            } break;

            case IT_SysCall: {
                Byte sysOpe = STACK_TOP.at(0);
                STACK_POP();

                switch(sysOpe) {
                    // enumを作る
                    case 0x00:
                    CONSOLE_OUT(STACK_TOP);
                    STACK_POP();
                    break;
                }
            } break;

            case IT_Block:
            return;

            case IT_Equal: {
                ByteVec value1 = STACK_TOP;
                STORE();
                ByteVec value2 = STACK_TOP;
                STORE();
                // Console::writeln(value1.toInt());
                // Console::writeln(value2.toInt());
                // Console::writeln(ByteSeq(value1 == value2).toHexString());
                STACK_PUSH(value1 == value2 ? ByteVec { 0x01 } : ByteVec { 0x00 });
            } break;

            case IT_Greater: {
                ByteVec value1 = STACK_TOP;
                STACK_POP();
                ByteVec value2 = STACK_TOP;
                STACK_POP();
                // Console::writeln(value1.toHexString());
                // Console::writeln(value2.toHexString());
                // Console::writeln(ByteVec(value1 < value2).toHexString());
                STACK_PUSH(value1 < value2 ? ByteVec { 0x01 } : ByteVec { 0x00 });
            } break;

            case IT_Jump: {
                ByteVec id = STACK_TOP;
                STACK_POP();

                if(id.size() == 16) {
                    Label label = this->labelList[id];

                    // if(id == this->labelList.findByName(ByteSeq("println")).id) {
                    //     CONSOLE_OUT(STACK_TOP);
                    //     STACK_POP();
                    //     break;
                    // }

                    this->index = label.beginIndex + 1;
                } else {
                    this->index += std::stoi(Interpreter::toHexString(id), nullptr, 16);
                    return;
                }
            } break;

            case IT_JumpIf: {
                ByteVec id = STACK_TOP;
                STACK_POP();
                ByteVec value = STACK_TOP;
                STACK_POP();

                // false (0x00) 以外の値はすべて通す
                if(value == ByteVec { 0x00 })
                    break;

                if(id.size() == 16 && this->labelList.count(id) == 1) {
                    Label label = this->labelList[id];
                    this->index = label.beginIndex;
                } else {
                    this->index = std::stoi(Interpreter::toHexString(id), nullptr, 16);
                    return;
                }
            } break;

            case IT_Load: {
                ByteVec countBytes;
                std::copy(INST.begin() + 1, INST.end(), std::back_inserter(countBytes));
                int count = std::stoi(Interpreter::toHexString(countBytes), nullptr, 16);

                for(int i = 0; i < count; i++)
                    LOAD();
            } break;

            case IT_Push: {
                ByteVec value;
                std::copy(INST.begin() + 1, INST.end(), std::back_inserter(value));

                STACK_PUSH(value);
            } break;

            case IT_Pop: {
                STACK_POP();
            } break;

            default: {
            } break;
        }

        this->index++;
        NEXT_INST_RUN();
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}
