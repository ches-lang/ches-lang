#pragma once

#include "bytecode.hpp"



FuncData::FuncData() {}

FuncData::FuncData(ByteSeq i, ByteSeq nm) {
    id = i;
    name = nm;
}

FuncData::FuncData(ByteSeq i, ByteSeq nm, int st, int ed) {
    id = i;
    name = nm;
    st = start;
    ed = end;
}



Instruction::Instruction(int opcode) {
    this->opcode = opcode;
    this->operand = {};
    this->setBytecode();
}

Instruction::Instruction(int opcode, std::unordered_map<std::string, ByteSeq> operand) {
    this->opcode = opcode;
    this->operand = operand;
    this->setBytecode();
}

void Instruction::setBytecode() {
    try {
        switch(this->opcode) {
            case IT_Unknown: {
                append(IT_Unknown);
            } break;

            case IT_Label: {
                append(IT_Label);
                append(this->operand["id"]);
                append(this->operand["name"]);
            } break;

            case IT_IFJump: {
                append(IT_IFJump);
                append(this->operand["index"]);
            } break;
        }
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void Instruction::append(Byte byte) {
    this->bytecode.push_back(byte);
}

void Instruction::append(ByteSeq bytes) {
    for(Byte b : bytes)
        this->bytecode.push_back(b);
}



Bytecode::Bytecode() {}

Bytecode::Bytecode(ByteSeq source) {
    this->source = source;
}

Bytecode::Bytecode(int source) {
    // todo: 256以上の数字にも対応させる
    this->source.push_back((Byte)source);
}

Bytecode::Bytecode(std::string source) {
    for(Byte src : source)
        this->source.push_back(src);
}

Bytecode::Bytecode(Node tree) {
    this->append(MAGIC_NUMBER);

    for(Node node : tree.children)
        if(node.type == ND_DefFunc)
            funcdata.push_back(FuncData(Bytecode(this->generateUUID()).source, Bytecode(node.tokenAt(0).string).source));

    int index = 0;
    InstList instList = this->toInstList(tree, index);

    for(Instruction inst : instList) {
        this->append(inst.bytecode);
        this->append(IT_LineDiv);
    }

    this->source.pop_back();
    std::cout << std::endl;
}

Bytecode::Bytecode(std::vector<Bytecode> source) {
    for(Bytecode bc : source) {
        this->append(Bytecode(MAGIC_NUMBER));
        //this->append(Bytecode(ByteSeq { IT_LineDivide, IT_BytecodeStart, IT_LineDivide }));
        this->append(Bytecode(IT_LineDiv));
        this->append(Bytecode(bc));
    }
}

Bytecode Bytecode::append(Byte source) {
    this->source.push_back(source);
    return this->source;
}

Bytecode Bytecode::append(Bytecode source) {
    for(Byte srcChar : source.source)
        this->source.push_back(srcChar);

    return this->source;
}

Bytecode Bytecode::pop() {
    this->source.pop_back();
    return this->source;
}

int Bytecode::toInt() {
    return *(unsigned int *)&this->source;
}

std::string Bytecode::toString() {
    std::string res;
    for(Byte srcChar : this->source)
        res.push_back(srcChar);
    return res;
}

// LineSeq型の命令を取得
LineSeq Bytecode::divide() {
    LineSeq res = {{{}}};

    for(int i = 128; i < source.size(); i++) {
        switch(this->source[i]) {
            case IT_LineDiv: {
                if(i + 1 < this->source.size() && this->source[i + 1] == IT_LineDiv) {
                    res.back().back().push_back(IT_LineDiv);
                    i++;
                } else {
                    res.push_back({{}});
                }
            } break;

            case IT_TokenDiv: {
                if(i + 1 < this->source.size() && this->source[i + 1] == IT_TokenDiv) {
                    res.back().back().push_back(IT_TokenDiv);
                    i++;
                } else {
                    res.back().push_back({});
                }
            } break;

            default: {
                res.back().back().push_back(this->source[i]);
            } break;
        }
    }

    return res;
}

// ノードを調べてバイトコードに変換し、LineSeq型の行列を返します
// 基本的に scanNode(Node) により呼ばれます
InstList Bytecode::toInstList(Node parentNode, int &index) {
    Node node = parentNode.childAt(index);
    std::cout << std::endl << index << " > "<< parentNode.children.size() << " | " << (int)node.type << std::endl;
    InstList instList;

    try {

        switch(node.type) {
            case ND_Unknown: {std::cout<<"unknown"<<std::endl;
                instList.push_back(Instruction(IT_Unknown));
            } break;

            case ND_Root: {std::cout<<"root"<<std::endl;
                int i = 0;
                InstList resLines = this->toInstList(node, i);
                std::copy(resLines.begin(), resLines.end(), std::back_inserter(instList));
            } break;

            case ND_DefVar: {std::cout<<"defvar"<<std::endl;
                instList.push_back(Instruction(IT_LSPush, {}));//{}
                int i = 1;
                this->toInstList(node, i);
                lslen++;
            } break;

            case ND_InitVar: {std::cout<<"initvar"<<std::endl;
                Token token = node.tokenAt(2);
                ByteSeq value;

                if(token.type == TK_Number)
                    value = Bytecode(std::stoi(token.string)).source;
                else if(token.type == TK_String)
                    value = Bytecode(token.string).source;

                instList.push_back(Instruction(IT_LSPush, { { "index", value } }));
                lslen++;
            } break;

            case ND_DefFunc: {std::cout<<"deffunc"<<std::endl;
                ByteSeq funcname = Bytecode(node.tokenAt(0).string).source;
                ByteSeq funcid = Bytecode(FuncData::findByName(funcdata, funcname).id).source;
                instList.push_back(Instruction(IT_Label, { { "id", funcid }, { "name", funcname } }));

                lllen += node.childAt(0).children.size();

                int i = 2;
                InstList insts = this->toInstList(node, i);
                std::copy(insts.begin(), insts.end(), std::back_inserter(instList));
            } break;

            case ND_CallFunc: {std::cout<<"callfunc"<<std::endl;
                ByteSeq funcname = Bytecode(node.tokenAt(0).string).source;
                instList.push_back(Instruction(IT_Jump, { { "id", FuncData::findByName(funcdata, funcname).id } }));
            } break;

            case ND_If: {std::cout<<"if"<<std::endl;
                // 次のノードに else / elseif がくれば

                if(index + 1 < parentNode.children.size()) {
                    int nextNodeType = parentNode.childAt(index + 1).type;

                    if(nextNodeType == ND_Else || nextNodeType == ND_ElseIf) {
                        //
                        std::cout<<"before else or elseif"<<std::endl;
                    } else {
                        std::cout << "not before else or elseif"<<nextNodeType<<std::endl;
                    }
                }

                // 条件式をチェック

                int i = 1;
                InstList procLines = this->toInstList(node, i);

                // procLineの行数をもとにIFJUMP命令を追加
                Bytecode lineIndex(IT_VarPref);
                lineIndex.append(Bytecode(procLines.size()));

                instList.push_back(Instruction(IT_IFJump, { { "index", lineIndex.source } }));

                std::copy(procLines.begin(), procLines.end(), std::back_inserter(instList));
            } break;

            case ND_Else: {std::cout<<"else"<<std::endl;
                int i = 0;
                InstList insts = this->toInstList(node, i);
                std::copy(insts.begin(), insts.end(), std::back_inserter(instList));
            } break;
        }

        return instList;

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
        return instList;
    }
}

Byte Bytecode::generateUUID() {
    uuid_t uuid;
    uuid_generate(uuid);
    return *uuid;
}
