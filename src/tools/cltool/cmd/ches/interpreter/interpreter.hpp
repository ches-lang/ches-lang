#pragma once

#undef  INST

#define BYTE                        (BYTE_AT(this->index))
#define BYTE_AT(index)              (this->bytes.at(index))
#define BYTE_LEN                    (this->bytes.size())
#define BYTE_TO_HEX(value)          (Interpreter::toHexString(value))
#define BYTE_TO_HEX_SEP(value, sep) (Interpreter::toHexString(value, sep))
#define BYTE_TO_INT(value)          (std::stoi(BYTE_TO_HEX(value), nullptr, 16))

#define CONSOLE_OUT(value)  Console::writeln("* print\n    " + BYTE_TO_HEX(value));

#define LOAD()              this->opeStack.top().push(this->stack.top().top());
#define NEXT_INST_RUN()     this->runNextInst();
#define STACK_POP()         this->stack.top().pop();
#define STACK_PUSH(value)   this->stack.top().push(value);
#define STACK_TOP           (this->stack.top().top())
#define STORE()             this->opeStack.top().pop();

#define L_AT(lineIndex)                     (lines.at(lineIndex))
#define L_TOKEN_AT(lineIndex, tokenIndex)   (lines.at(lineIndex).at(tokenIndex))

typedef std::stack<ches::ByteVec>   Stack;

#include "interpreter.cpp"


ches::Block::Block() {}

ches::Block::Block(ByteVec name, int beginIndex) {
    this->name = name;
    this->beginIndex = beginIndex;
}


ches::Interpreter::Interpreter(std::string filePath) {
    try {
        this->loadCompiledFile(filePath);

        for(auto [ id, block ] : this->blockList) {
            Console::writeln("- block -");
            Console::writeln("  * id:\t" + BYTE_TO_HEX(id));
            Console::writeln("  * name:\t" + BYTE_TO_HEX(block.name));
            Console::writeln("  * begin:\t" + std::to_string(block.beginIndex));
            Console::writeln();
        }

        // ヘッダ長をチェック
        // if(this->header.size() != HEADER_LEN)
        //     Console::log(LogType_Error, 5173, { { "Path", filePath } });

        // ヘッダとボディを取得
        // this->header = this->source.copy(0, HEADER_LEN - 1);
        // this->body = this->source.copy(HEADER_LEN, -1);

        // this->headerInfo = HeaderInfo(this->header);
        // LineSeq lines = ByteSeq(this->body).toLineSeq();

        // 命令リストを取得
        // for(ByteSeq bytes : lines)
            // this->instList.push_back(Instruction(bytes));

        // バイトコード&アセンブリコードのログを出力

        // std::vector<std::string> rawLines;
        // std::vector<std::string> asmLines;

        // ByteVec tmpLine;

        // for(int i = 0; i < BYTE_LEN; i++) {
        //     std::string index = std::to_string(i);

        //     while(index.length() < 3)
        //         index = "0" + index;

        //     // std::string line = BYTE_TO_HEX(this->instList.at(i), " ");

        //     // if(this->bytes.at(i) == IT_InstDiv) {
        //     //     if(i + 1 < BYTE_LEN && this->bytes.at(i + 1) == IT_InstDiv) {
        //     //         tmpLine.push_back(IT_InstDiv);
        //     //         i++;
        //     //     } else {
        //     //         tmpLine.clear();
        //     //         rawLines.push_back(index + "| " + BYTE_TO_HEX(tmpLine, " "));
        //     //     }
        //     // } else {
        //     //     tmpLine.push_back(this->bytes.at(i));
        //     // }
        // }

        // rawLines.push_back(index + "| " + BYTE_TO_HEX(tmpLine, " "));

        // for(int i = 0; i < this->instList.size(); i++) {
        //     std::string index = std::to_string(i);

        //     while(index.length() < 3)
        //         index = "0" + index;

        //     asmLines.push_back(index + "| " + Interpreter::instToString(this->instList.at(i)));
        // }

        // Console::printDebugLog("raw bytecode", rawLines);
        // Console::printDebugLog("assembly code", asmLines);

        // マジックナンバーをチェック
        if(this->magicNum != MAGIC_NUM)
            Console::log(LogType_Error, 8732, { { "Path", filePath } }, true);

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

ches::ByteVec ches::Interpreter::copyBytesUntilDiv(int beginIndex) {
    ByteVec result;

    for(int i = beginIndex; i < BYTE_LEN; i++) {
        if(BYTE_AT(i) == IT_InstDiv) {
            if(i + 1 < BYTE_LEN && BYTE_AT(i + 1) == IT_InstDiv) {
                result.push_back(IT_InstDiv);
                i++;
            } else {
                break;
            }
        } else {
            result.push_back(BYTE_AT(i));
        }
    }

    return result;
}

std::vector<ches::ByteVec> ches::Interpreter::divideInsts() {
    std::vector<ByteVec> result;
    ByteVec tmpLine;

    // for(int i = 0; i < this->body.size(); i++) {
    //     if(this->body.at(i) == IT_InstDiv || i == this->body.size() - 1) {
    //         if(i + 1 < this->body.size() && this->body.at(i + 1) == IT_InstDiv) {
    //             tmpLine.push_back(IT_InstDiv);
    //             i++;
    //             continue;
    //         }

    //         result.push_back(tmpLine);
    //         tmpLine.clear();
    //         continue;
    //     }

    //     tmpLine.push_back(this->body.at(i));
    // }

    return result;
}

void ches::Interpreter::loadCompiledFile(std::string filePath) {
    try {
        std::ifstream ifs(filePath);

        if(!ifs.is_open())
            Console::log(LogType_Error, 327, { { "Path", filePath } }, true);

        if(ifs.fail())
            Console::log(LogType_Error, 6845, { { "Path", filePath } }, true);

        Byte byte;

        do {
            ifs.read((char*)&byte, sizeof(char));
            this->bytes.push_back(byte);
        } while(!ifs.eof());

        /*int loadedSize = 0;

        do {
            ifs.read((char*)&byte, sizeof(char));
            this->header.push_back(byte);
            loadedSize++;
        } while(!ifs.eof() && loadedSize < HEADER_LEN);

        ByteVec idAreaIndexBytes;
        Console::writeln(BYTE_TO_HEX(this->header, " "));

        for(int i = 8; i < 16; i++) {
            if(this->header.at(i) == IT_InstDiv)
                break;

            idAreaIndexBytes.push_back(this->header.at(i));
        }

        // std::copy(this->header.begin() + 8, this->header.end() - this->header.size() + 16, std::back_inserter(idAreaIndexBytes));
        Console::writeln("l");
        Console::writeln(BYTE_TO_HEX(idAreaIndexBytes));
        Console::writeln("b");
        int idAreaIndex = std::stoi(BYTE_TO_HEX(idAreaIndexBytes), nullptr, 16);
        Console::writeln("a");
        Console::writeln(BYTE_TO_HEX(idAreaIndexBytes));
        Console::writeln("a");

        do {
            ifs.read((char*)&byte, sizeof(char));
            this->body.push_back(byte);
            loadedSize++;
        } while(!ifs.eof() && loadedSize < idAreaIndex);

        do {
            ifs.read((char*)&byte, sizeof(char));
            this->idArea.push_back(byte);
            loadedSize++;
        } while(!ifs.eof());

        Console::writeln("aaaa");
        Console::writeln(this->header.size());
        Console::writeln(this->body.size());
        Console::writeln(this->idArea.size());

        // if(result.size() > 0)
        //     result.pop_back();

        int magicNumSize = MAGIC_NUM.size();
        std::copy(this->header.begin(), this->header.end() - this->header.size() + 8, std::back_inserter(this->magicNum));

        this->instList = Interpreter::divideInsts();
        */

        if(this->bytes.size() > 0)
            this->bytes.pop_back();

        ifs.close();

        // ヘッダ情報をロード

        int loadIndex = 0;

        for(int i = 0; i < MAGIC_NUM.size(); i++)
            this->magicNum.push_back(BYTE_AT(i));

        ByteVec idAreaIndexBytes = this->copyBytesUntilDiv(MAGIC_NUM.size());
        this->idAreaIndex = BYTE_TO_INT(idAreaIndexBytes);

        // ID情報をロード

        for(int i = this->idAreaIndex; i < BYTE_LEN; ) {
            ByteVec id;

            for(int j = 0; j < 16; i++, j++)
                id.push_back(BYTE_AT(i));

            ByteVec name = this->copyBytesUntilDiv(i);
            i += name.size() + 1;

            ByteVec beginIndexBytes = this->copyBytesUntilDiv(i);
            int beginIndex = BYTE_TO_INT(beginIndexBytes);
            i += beginIndexBytes.size() + 1;

            this->blockList[id] = Block(name, beginIndex);
        }
    } catch(std::exception excep) {
        Console::log(LogType_Error, 6845, { { "Path", filePath } }, true);
    }
}

void ches::Interpreter::printStackLog() {
    try {
        Console::writeln();
        Console::write("[stacktop] " + std::to_string(BYTE_TO_INT(STACK_TOP)) + " | 0x" + BYTE_TO_HEX_SEP(STACK_TOP, " 0x"));

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void ches::Interpreter::runProgram() {
    try {
        // ブロックデータを設定
        // this->setBlockData();

        // ブロック一覧のログを出力

        // std::vector<std::string> logLines;

        // for(auto [ id, block ] : this->blockList)
        //     logLines.push_back("[" + BYTE_TO_HEX(id) + "] " + BYTE_TO_HEX(block.name));///name

        // Console::printDebugLog("block list", logLines);

        // スタックの初期化

        this->stack.push(Stack());
        this->opeStack.push(Stack());

        // // エントリポイントの呼び出し

        Block entryBlock;

        // エントリポイントのラベルを取得
        for(auto [ id, block ] : this->blockList)
            if(block.name == ByteVec { 0x63, 0x6F, 0x6E, 0x73, 0x74 })
                entryBlock = block;

        this->index = entryBlock.beginIndex;

        Console::writeln("index\tbyte\topcode\t\tstacktop\tinst");
        NEXT_INST_RUN();
        Console::writeln();
        Console::writeln();
        Console::writeln();

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void ches::Interpreter::setBlockData() {
    try {/*
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
                std::copy(inst.begin() + 1, inst.end() - inst.size() + 17, std::back_inserter(id));;
                ByteVec name;
                std::copy(inst.begin() + 18, inst.end(), std::back_inserter(name));;

                this->blockList[id] = Block(id, name, beginIndex, endIndex);
            } else {
                // ラベル外の処理
            }
        }*/
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void ches::Interpreter::runNextInst() {
    try {
        std::string hexOpcode = instTypeMap.count(BYTE) == 1 ? instTypeMap.at(BYTE) : "OOR";
        std::string hexStackTop = this->stack.empty() || this->stack.top().empty() ? "noelem" : BYTE_TO_HEX_SEP(STACK_TOP, " ");
        Console::write(std::to_string(this->index) + "\t0x" + BYTE_TO_HEX(ByteVec { BYTE }) + "\t" + hexOpcode + "\t\t" + hexStackTop + "\t\t");

        if(this->index >= BYTE_LEN)
            return;

        Byte opcode = BYTE;
        Byte opcodeIndex = this->index;

        // opcode分をindexに追加する
        this->index++;

        switch(opcode) {
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

            case IT_Ret:
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
                    Block block = this->blockList[id];

                    // if(id == this->blockList.findByName(ByteSeq("println")).id) {
                    //     CONSOLE_OUT(STACK_TOP);
                    //     STACK_POP();
                    //     break;
                    // }

                    this->index = block.beginIndex;
                } else {
                    this->index += BYTE_TO_INT(id);
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

                if(id.size() == 16 && this->blockList.count(id) == 1) {
                    Block block = this->blockList[id];
                    this->index = block.beginIndex;
                } else {
                    this->index = BYTE_TO_INT(id);
                    return;
                }
            } break;

            case IT_Load: {
                ByteVec countBytes = this->copyBytesUntilDiv(this->index);
                this->index += countBytes.size();
                int count = BYTE_TO_INT(countBytes);

                for(int i = 0; i < count; i++)
                    LOAD();
            } break;

            case IT_Push: {
                // 範囲チェック入れる？
                DataSizeType size = static_cast<DataSizeType>(BYTE);
                this->index++;

                ByteVec value = this->copyBytesUntilDiv(this->index);
                this->index += value.size();

                STACK_PUSH(value);
            } break;

            case IT_Pop: {
                STACK_POP();
            } break;

            default:
            break;
        }

        ByteVec inst;

        // インデックスはINST_DIVの直前に合わせるようにすること
        this->index++;

        for(int i = opcodeIndex; i < this->index - 1; i++)
            inst.push_back(BYTE_AT(i));

        std::string hexInst = BYTE_TO_HEX_SEP(inst, " ");
        Console::writeln((hexInst == "" ? "[nobytes]" : hexInst));

        NEXT_INST_RUN();
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}
