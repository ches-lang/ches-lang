#pragma once

#undef  INST

#define BYTE                            (BYTE_AT(this->index))
#define BYTE_AT(index)                  (this->bytes[index])
#define BYTES_AT(indexPair)             (this->getBytes(indexPair))
#define BYTE_LEN                        (this->bytesLen)

#define BYTES_TO_BOOL(value)                (Interpreter::toBool(value))
#define BYTES_TO_HEX(size, value)           (Interpreter::toHexString(size, value))
#define BYTES_TO_HEX_SEP(size, value, sep)  (Interpreter::toHexString(size, value, sep))
#define BYTES_TO_INT(size, value)           (Interpreter::toInt(size, value))
#define BYTE_INDEXES_TO_INT(indexPair)      (Interpreter::toInt(indexPair))
#define TO_BYTES(value)                     (Interpreter::toBytes(value))
#define TO_BOOL_PTR(value)                  (Interpreter::toBoolPtr(value))

#define GET_DIV_INDEX(indexer)          (this->getBytesLenUntilDiv(indexer))

#define DATA_TYPE_SIZE                  (4)
#define SIZE_TYPE_TO_BIT(sizeType)      (dataSizeToBitSize.at(sizeType))

#define CONSOLE_OUT(value)  Console::writeln("* print\n    " + BYTE_TO_HEX(value));

#define STACK_BYTE_SIZE         (128)
#define STACK_EMPTY()           (this->stack.empty())
#define STACK_POP()             this->stack.pop();
#define STACK_PUSH(size, value) this->stack.push(size, value);
#define STACK_TOP               (this->stack.top())
#define STACK_TOP_SIZE          (this->stack.topSize())

#define OPESTACK_LEN        (this->opeStack.size())
#define OPESTACK_LOAD()     this->opeStack.push(STACK_TOP);
#define OPESTACK_UNLOAD()    this->opeStack.pop();
#define OPESTACK_TOP        (this->opeStack.top())

#define L_AT(lineIndex)                     (lines.at(lineIndex))
#define L_TOKEN_AT(lineIndex, tokenIndex)   (lines.at(lineIndex).at(tokenIndex))

typedef std::pair<int, int> IndexPair;

#include "interpreter.cpp"


ches::Block::Block() {}

ches::Block::Block(Byte *name, size_t nameSize, int beginIndex) {
    this->name = name;
    this->nameSize = nameSize;
    this->beginIndex = beginIndex;
}

bool ches::Block::compareName(Byte *name) {
    for(int i = 0; i < this->nameSize; i++)
        if(this->name[i] != name[i])
            return false;

    return true;
}


ches::DataStack::DataStack() {}


ches::Interpreter::Interpreter(std::string filePath) {
    try {
        this->loadCompiledFile(filePath);

        for(auto [ id, block ] : this->blockList) {
            Console::writeln("- block -");
            Console::writeln("  * id:\t" + BYTES_TO_HEX(16, id));
            Console::writeln("  * name:\t" + BYTES_TO_HEX(block.nameSize, block.name));
            Console::writeln("  * begin:\t" + std::to_string(block.beginIndex));
            Console::writeln();
        }

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

ches::Byte* ches::Interpreter::getBytes(IndexPair indexPair) {
    int size = sizeof(Byte) * indexPair.second;
    Byte *result = (Byte*)malloc(size);

    for(int i = 0; i < indexPair.second; i++)
        result[i] = BYTE_AT(indexPair.first + i);

    return result;
}

IndexPair ches::Interpreter::getBytesLenUntilDiv(int &indexer) {
    int beginIndex = indexer;

    try {
        for(; indexer < BYTE_LEN; indexer++) {
            if(BYTE_AT(indexer) == IT_InstDiv) {
                if(indexer + 1 < BYTE_LEN && BYTE_AT(indexer + 1) == IT_InstDiv) {
                    indexer++;
                } else {
                    break;
                }
            }
        }
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION on Interpreter::getBytesLenUntilDiv()" << std::endl;
    }

    return { beginIndex, indexer - beginIndex };
}

void ches::Interpreter::finalize() {
    this->stack.clear();
    safe_free(this->bytes);
    exit(-1);
}

void ches::Interpreter::loadCompiledFile(std::string filePath) {
    try {
        std::ifstream ifs(filePath);

        if(!ifs.is_open()) {
            Console::log(LogType_Error, 327, { { "Path", filePath } });
            this->finalize();
        }

        if(ifs.fail()) {
            Console::log(LogType_Error, 6845, { { "Path", filePath } });
            this->finalize();
        }

        Byte byte;

        do {
            ifs.read((char*)&byte, sizeof(char));
            this->bytes[BYTE_LEN++] = byte;
        } while(!ifs.eof());

        if(BYTE_LEN > 0)
            this->bytes[--BYTE_LEN] = 0;

        ifs.close();

        // ヘッダ情報をロード

        // マジックナンバーをチェック
        Byte magicNum[] = MAGIC_NUM;

        for(int i = 0; i < MAGIC_NUM_LEN; i++) {
            if(BYTE_AT(i) != magicNum[i]) {
                Console::log(LogType_Error, 8732, { { "Path", filePath } });
                this->finalize();
            }
        }

        int idAreaIndexer = MAGIC_NUM_LEN;
        this->idAreaIndex = BYTE_INDEXES_TO_INT(GET_DIV_INDEX(idAreaIndexer));

        // ID情報をロード

        try {
            for(int i = this->idAreaIndex; i < BYTE_LEN; ) {
                Byte id[16];

                for(int j = 0; j < 16; i++, j++)
                    id[j] = BYTE_AT(i);

                std::pair<int, int> indexes = GET_DIV_INDEX(i);
                Byte *name = BYTES_AT(indexes);
                i++;

                int beginIndex = BYTE_INDEXES_TO_INT(GET_DIV_INDEX(i));
                i++;

                this->blockList[id] = Block(name, indexes.second, beginIndex);
            }
        } catch(std::exception excep) {
            // todo: ID情報のエラーを追加
            Console::writeln("ID area data is invalid.");
        }
    } catch(std::exception excep) {
        Console::log(LogType_Error, 6845, { { "Path", filePath } });
        this->finalize();
    }
}

void ches::Interpreter::printDetails() {
    std::cout << std::endl;

    int idAreaLen = this->bytesLen - this->idAreaIndex;

    std::cout << "sumBytesLen: " << BYTE_LEN << std::endl;
    std::cout << "headerLen: " << HEADER_LEN << " (0~" << HEADER_LEN - 1 << ")" << std::endl;
    std::cout << "idAreaLen: " << idAreaLen << " (" << this->idAreaIndex << "~" << this->bytesLen - 1 << ")" << std::endl;
    std::cout << std::endl;

    std::cout << "startIndex: " << this->index << std::endl;
    std::cout << std::endl;

    Byte magicNum[] = MAGIC_NUM;

    std::cout << "magicNum: " << BYTES_TO_HEX_SEP(MAGIC_NUM_LEN, TO_BYTES(magicNum), " ") << std::endl;
    std::cout << std::endl;
}

int ches::Interpreter::toInt(IndexPair indexes) {
    int result = 0;

    int begin = indexes.first;
    int size = indexes.second;

    int digit = size * 8;
    bool digitStarted = false;

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < 8; j++) {
            bool bit = (BYTE_AT(begin + i) >> 8 - j - 1) & 1;
            digit--;

            if(!digitStarted) {
                if(bit == 1) {
                    digitStarted = true;
                } else {
                    continue;
                }
            }

            result += (bit ? std::pow(2, digit) : 0);
        }
    }

    return result;
}

void ches::Interpreter::runProgram() {
    try {
        // エントリポイントの呼び出し

        Block entryBlock;
        Byte entryBlockName[] = { 0x63, 0x6F, 0x6E, 0x73, 0x74 };
        int entryBlockNameSize = 5;

        // エントリポイントのラベルを取得
        for(auto [ id, block ] : this->blockList)
            if(block.compareName(entryBlockName))
                entryBlock = block;

        if(entryBlock.empty()) {
            Console::writeln("Entry point was not found.");
            Console::writeln("( name: " + BYTES_TO_HEX_SEP(5, entryBlock.name, " ") + " )");
            return;
        }

        this->index = entryBlock.beginIndex;

        this->printDetails();
        Console::writeln();

        if(this->index < HEADER_LEN || this->index >= this->idAreaIndex) {
            Console::writeln("Index is invalid.");
            return;
        }

        Console::writeln("index\tbyte\topcode\toslen\tstacktop\trawinst");
        Console::writeln();

        while(this->index < this->idAreaIndex)
            this->runNextInst();

        std::string hexStackTop = (this->stack.empty()) ? "noelem" : BYTES_TO_HEX_SEP(STACK_TOP_SIZE, STACK_TOP, " ");
        Console::writeln("<end>\t\t\t\t" + hexStackTop);

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }

    this->finalize();
}

void ches::Interpreter::runNextInst() {
    try {
        if(this->index < HEADER_LEN || this->index >= this->idAreaIndex) {
            Console::writeln("Index is invalid. / runNextInst()");
            return;
        }

        Byte opcode = BYTE;
        Byte opcodeIndex = this->index;

        // opcode分をindexに追加する
        this->index++;

        std::string hexOpcode = instTypeMap.count(opcode) == 1 ? instTypeMap.at(opcode) : "OOR";
        std::string hexStackTop = this->stack.empty() ? "noelem\t" : BYTES_TO_HEX_SEP(STACK_TOP_SIZE, STACK_TOP, " ");
        Console::write(std::to_string(this->index + 1) + "\t0x" + BYTES_TO_HEX(1, TO_BYTES(opcode)) + "\t" + hexOpcode + "\t" + std::to_string(OPESTACK_LEN) + "\t" + hexStackTop + "\t");

        switch(opcode) {
            case IT_InstDiv: {
                // 表示調節のために改行
                Console::writeln();
                Console::writeln();
                Console::log(LogType_Error, 7316, { { "Index", std::to_string(this->index - 1) } });
                this->finalize();
            } break;

            case IT_Add: {
                Byte size = STACK_TOP_SIZE;

                Byte *value1 = OPESTACK_TOP;
                OPESTACK_UNLOAD();

                Byte *value2 = OPESTACK_TOP;
                OPESTACK_UNLOAD();

                int result = BYTES_TO_INT(STACK_TOP_SIZE, value1) + BYTES_TO_INT(STACK_TOP_SIZE, value2);
                STACK_PUSH(size, TO_BYTES(result));
            } break;

            case IT_Load: {
                int count = BYTE_INDEXES_TO_INT(GET_DIV_INDEX(this->index));
                this->stack.loadTo(count, this->opeStack);
            } break;

            case IT_Push: {
                // 範囲チェック入れる？
                Byte size = BYTE;
                this->index++;

                std::pair<int, int> indexes = GET_DIV_INDEX(this->index);
                STACK_PUSH(size, BYTES_AT(indexes));
            } break;

            case IT_Pop: {
                STACK_POP();
            } break;
        }

        /*switch(opcode) {
            case IT_InstDiv: {
                // 表示調節のために改行
                Console::writeln();
                Console::writeln();
                Console::log(LogType_Error, 7316, { { "Index", std::to_string(this->index - 1) } });
                this->finalize();
            } break;

            case IT_Add: {
                Byte size = STACK_TOP_SIZE;

                Byte *value1 = OPESTACK_TOP;
                OPESTACK_UNLOAD();

                Byte *value2 = OPESTACK_TOP;
                OPESTACK_UNLOAD();

                int result = BYTES_TO_INT(STACK_TOP_SIZE, value1) + BYTES_TO_INT(STACK_TOP_SIZE, value2);

                STACK_PUSH(size, TO_BYTES(result));
            } break;

            case IT_Equal: {
                Byte size = STACK_TOP_SIZE;

                Byte *value1 = OPESTACK_TOP;
                OPESTACK_UNLOAD();

                Byte *value2 = OPESTACK_TOP;
                OPESTACK_UNLOAD();

                STACK_PUSH(size, TO_BYTES(value1 == value2));
            } break;

            case IT_Jump: {
                IndexPair indexPair = GET_DIV_INDEX(this->index);
                Byte *indexPtr = BYTES_AT(indexPair);

                Console::write("<");
                Console::write(BYTE_INDEXES_TO_INT(indexPair));
                Console::write("> ");

                if(indexPair.second == 16 && this->blockList.count(indexPtr) == 1) {
                    Block block = this->blockList.at(indexPtr);
                    this->index = block.beginIndex;
                } else {
                    this->index += BYTE_INDEXES_TO_INT(indexPair);

                    if(this->index + 1 > this->idAreaIndex || this->index + 1 < HEADER_LEN) {
                        // 表示調節のために改行
                        Console::writeln();
                        Console::writeln();
                        Console::log(LogType_Error, 2947, { { "Index", std::to_string(this->index + 1) } });
                        this->finalize();
                    }
                }
            } break;

            case IT_JumpIf: {
                Byte *value = OPESTACK_TOP;
                OPESTACK_UNLOAD();

                IndexPair indexPair = GET_DIV_INDEX(this->index);
                Byte *indexPtr = BYTES_AT(indexPair);

                if(!BYTES_TO_BOOL(value))
                    break;

                if(indexPair.second == 16 && this->blockList.count(indexPtr) == 1) {
                    Block block = this->blockList.at(indexPtr);
                    this->index = block.beginIndex;
                } else {
                    Console::write("<");
                    Console::write(BYTE_INDEXES_TO_INT(indexPair));
                    Console::write("> ");

                    this->index += BYTE_INDEXES_TO_INT(indexPair);

                    if(this->index + 1 > this->idAreaIndex || this->index + 1 < HEADER_LEN) {
                        // 表示調節のために改行
                        Console::writeln();
                        Console::writeln();
                        Console::log(LogType_Error, 2947, { { "Index", std::to_string(this->index + 1) } });
                        this->finalize();
                    }
                }
            } break;

            default: {
                // 無効な命令なのでエラー
                // 表示調節のために改行
                Console::writeln();
                Console::writeln();
                Console::log(LogType_Error, 7316, { { "Index", std::to_string(this->index - 1) } });
                this->finalize();
            } break;
        }*/

        /* 旧コード
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
                    this->index += BYTES_TO_INT(id);
                    return;
                }
            } break;

            case IT_JumpIf: {
                ByteVec value = STACK_TOP;
                STACK_POP();

                ByteVec indexVec = this->copyBytesUntilDiv(this->index);

                // false (0x00) 以外の値はすべて通す
                if(value == ByteVec { 0x00 })
                    break;

                if(indexVec.size() == 16 && this->blockList.count(indexVec) == 1) {
                    Block block = this->blockList.at(indexVec);
                    this->index = block.beginIndex;
                } else {
                    this->index += BYTES_TO_INT(indexVec);
                }
            } break;

            case IT_Load: {
                ByteVec countBytes = this->copyBytesUntilDiv(this->index);
                int count = BYTES_TO_INT(countBytes);

                for(int i = 0; i < count; i++)
                    LOAD();
            } break;

            case IT_Push: {
                // 範囲チェック入れる？
                DataSizeType size = static_cast<DataSizeType>(BYTE);
                this->index++;

                ByteVec value = this->copyBytesUntilDiv(this->index);

                STACK_PUSH(value);
            } break;

            case IT_Pop: {
                STACK_POP();
            } break;

            default:
            break;
        }
        */

        int size = this->index - opcodeIndex;

        // インデックスをINST_DIVの直前に合わせる
        this->index++;

        Byte* inst = (Byte*)malloc(sizeof(Byte) * size);

        for(int i = 0; i < size; i++)
            inst[i] = BYTE_AT(i);

        std::string hexInst = BYTES_TO_HEX_SEP(size, inst, " ");
        Console::writeln((hexInst == "" ? "[nobytes]" : hexInst));

        safe_free(inst);

    } catch(std::out_of_range ignored) {
        // 表示調節のために改行
        Console::writeln();
        Console::writeln();
        Console::log(LogType_Error, 0, { { "At", "Interpreter::runNextInst()" } });
        this->finalize();
    }
}
