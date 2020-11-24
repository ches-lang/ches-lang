#pragma once

#undef  INST

#define CONSOLE_OUT(value)  do { Console::writeln("* print\n    " + value.toString() + " (" + value.toHexString() + ")"); this->printStackLog(); } while(0)
#define INST                (this->instList.at(this->index))
#define LOAD()              do { this->opeStack.top().push(this->stack.top().top()); Console::writeln("* load\n    " + std::to_string(this->opeStack.top().size())); this->printStackLog(); } while(0)
#define NEXT_INST_RUN()     do { this->runNextInst(); /* Console::writeln("* run: " + InstConv::toString(inst)); this->printStackLog(); */ } while(0)
#define STACK_POP()         do { this->stack.top().pop(); Console::writeln("* pop\n    " + std::to_string(this->stack.top().size())); this->printStackLog(); } while(0)
#define STACK_PUSH(value)   do { this->stack.top().push(value); Console::writeln("* push\n    " + std::to_string(this->stack.top().size()) + " " + value.toHexString()); this->printStackLog(); } while(0)
#define STACK_TOP           (this->stack.top().top())
#define STORE()             do { this->opeStack.top().pop(); Console::writeln("* store\n    " + std::to_string(this->opeStack.top().size())); this->printStackLog(); } while(0)

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

        for(Function label : this->labelList)
            logLines.push_back("[" + label.id.toHexString() + "] " + label.name.toString());

        Console::printDebugLog("label list", logLines);

        // エントリポイントの呼び出し

        this->list.push(std::list<ByteSeq>());
        this->stack.push(std::stack<ByteSeq>());
        this->opeStack.push(std::stack<ByteSeq>());

        Function label = this->labelList.findByName(ByteSeq("const"));
        this->index = label.begin + 1;
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
            Instruction inst = this->instList.at(i);

            if(inst.opcode == IT_Block) {
                // ラベルの終了インデックスと内容を取得
                InstList labelInstList;
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

                // int endIndex = i;

                ByteSeq id = inst.operand.at(0);
                ByteSeq name = inst.operand.at(1);

                this->labelList.push_back(Function(id, name, beginIndex, -1));
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
        Console::writeln(instTypeMap.at(INST.opcode));
        Console::writeln();

        if(this->index >= this->instList.size())
            return;

        switch(INST.opcode) {
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
                ByteSeq value1 = STACK_TOP;
                STORE();
                ByteSeq value2 = STACK_TOP;
                STORE();
                Console::writeln(value1.toInt());
                Console::writeln(value2.toInt());
                Console::writeln(ByteSeq(value1 == value2).toHexString());
                STACK_PUSH(ByteSeq(value1 == value2));
            } break;

            case IT_Greater: {
                ByteSeq value1 = STACK_TOP;
                STACK_POP();
                ByteSeq value2 = STACK_TOP;
                STACK_POP();
                Console::writeln(value1.toHexString());
                Console::writeln(value2.toHexString());
                Console::writeln(ByteSeq(value1 < value2).toHexString());
                STACK_PUSH(ByteSeq(value1 < value2));
            } break;

            case IT_Jump: {
                ByteSeq index = STACK_TOP;
                STACK_POP();

                if(index.size() == 16) {
                    Function label = this->labelList.findById(index);

                    if(index == this->labelList.findByName(ByteSeq("println")).id) {
                        CONSOLE_OUT(STACK_TOP);
                        STACK_POP();
                        break;
                    }

                    this->index = label.begin + 1;
                } else {
                    this->index += index.toInt();
                    return;
                }
            } break;

            case IT_JumpIf: {
                ByteSeq index = STACK_TOP;
                STACK_POP();
                ByteSeq value = STACK_TOP;
                STACK_POP();

                // false (0x00) 以外の値はすべて通す
                if(value == (ByteSeq){ 0x00 })
                    break;

                if(index.size() == 16 && this->labelList.existsId(index)) {
                    Function label = this->labelList.findById(index);
                    this->index = label.begin;
                } else {
                    this->index += index.toInt();
                    return;
                }
            } break;

            case IT_Load: {
                int count = INST.operand.at(0).toInt();

                for(int i = 0; i < count; i++)
                    LOAD();
            } break;

            case IT_Push: {
                STACK_PUSH(INST.operand.at(1));
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
