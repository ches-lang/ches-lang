#pragma once

#include "bytecode.hpp"



FuncData_::FuncData_() {}

FuncData_::FuncData_(std::vector<unsigned char> i, std::vector<unsigned char> nm) {
    id = i;
    name = nm;
}

FuncData_::FuncData_(std::vector<unsigned char> i, std::vector<unsigned char> nm, int st, int ed) {
    id = i;
    name = nm;
    st = start;
    ed = end;
}



Bytecode::Bytecode() {}

Bytecode::Bytecode(std::vector<unsigned char> source) {
    this->source = source;
}

Bytecode::Bytecode(int source) {
    // todo: 256以上の数字にも対応させる
    this->source.push_back((unsigned char)source);
}

Bytecode::Bytecode(std::string source) {
    for(unsigned char src : source)
        this->source.push_back(src);
}

Bytecode::Bytecode(Node tree) {
    this->source = this->toBytecode(tree).source;
}

Bytecode::Bytecode(std::vector<Bytecode> source) {
    for(Bytecode bc : source) {
        append(Bytecode("compiled_ches"));
        //append(Bytecode(std::vector<unsigned char> { IT_LineDivide, IT_BytecodeStart, IT_LineDivide }));
        append(Bytecode(IT_LineDivide));
        append(Bytecode(bc));
    }
}

Bytecode::Bytecode(std::vector<std::vector<std::vector<unsigned char>>> source) {
    for(int ln = 0; ln < source.size(); ln++) {
        for(int tk = 0; tk < source[ln].size(); tk++) {
            for(int ch = 0; ch < source[ln][tk].size(); ch++) {
                if(source[ln][tk][ch] == IT_LineDivide) this->append(IT_LineDivide);
                else if(source[ln][tk][ch] == IT_TokenDivide) this->append(IT_TokenDivide);
                this->append(source[ln][tk][ch]);
            }

            if(tk != source[ln].size() - 1) this->append(IT_TokenDivide);
        }

        if(ln != source.size() - 1) this->append(IT_LineDivide);
    }
}

Bytecode Bytecode::append(unsigned char source) {
    this->source.push_back(source);
    return this->source;
}

Bytecode Bytecode::append(Bytecode source) {
    for(unsigned char srcChar : source.source)
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
    for(unsigned char srcChar : this->source)
        res.push_back(srcChar);
    return res;
}

// vector型の命令を取得
std::vector<std::vector<std::vector<unsigned char>>> Bytecode::divide() {
    std::vector<std::vector<std::vector<unsigned char>>> res = {{{}}};

    for(int i = 0; i < source.size(); i++) {
        switch(this->source[i]) {
            case IT_LineDivide:
            if(i + 1 < this->source.size() && this->source[i + 1] == IT_LineDivide) {
                res.back().back().push_back(IT_LineDivide);
                i++;
            } else {
                res.push_back({{}});
            }
            break;

            case IT_TokenDivide:
            if(i + 1 < this->source.size() && this->source[i + 1] == IT_TokenDivide) {
                res.back().back().push_back(IT_TokenDivide);
                i++;
            } else {
                res.back().push_back({});
            }
            break;

            default:
            res.back().back().push_back(this->source[i]);
            break;
        }
    }

    return res;
}

Bytecode Bytecode::toBytecode(Node tree) {
    this->lines.push_back({ { 0x63, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x64, 0x5f, 0x63, 0x68, 0x65, 0x73 } });

    for(Node node : tree.children)
        if(node.type == ND_DefFunction)
            funcdata.push_back(FuncData(Bytecode(this->generateUUID()).source, Bytecode(node.tokenAt(0).string).source));

    while(index < tree.children.size()) {
        this->addBytecodeToNode(tree.childAt(index));
        index++;
    }

    return Bytecode(lines);
}

void Bytecode::addBytecodeToNode(Node node) {
    try {

        switch(node.type) {
            case ND_CallFunction: {
                std::vector<unsigned char> funcname = Bytecode(node.tokenAt(0).string).source;
                this->lines.push_back({ { IT_Jump }, FuncData::findByName(funcdata, funcname).id });
            } break;

            case ND_DefFunction: {
                std::vector<unsigned char> funcname = Bytecode(node.tokenAt(0).string).source;
                std::vector<unsigned char> funcid = Bytecode(FuncData::findByName(funcdata, funcname).id).source;
                this->lines.push_back({ { IT_Label }, funcid, funcname });

                if(node.children.size() > 1)
                    lllen += node.childAt(0).children.size();
            } break;

            case ND_DefVariable: {
                this->lines.push_back({ { IT_LocalStackPush }, {} });
                lslen++;
            } break;

            case ND_If: {
                this->lines.push_back({ { IT_IFJump }, {  } });

                for(; index < node.childAt(1).children.size(); index++)
                    //this->lines.push_back(this->nodeToBytecode(node.childAt(1).childAt(index)));
                    this->addBytecodeToNode(node.childAt(1).childAt(index));

                index--;
            } break;

            case ND_InitVariable: {
                Token token = node.tokenAt(2);
                std::vector<unsigned char> value;

                if(token.type == TK_Number)
                    value = Bytecode(std::stoi(token.string)).source;
                else if(token.type == TK_String)
                    value = Bytecode(token.string).source;

                this->lines.push_back({ { IT_LocalStackPush }, value });
                lslen++;
            } break;
        }

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

unsigned char Bytecode::generateUUID() {
    uuid_t uuid;
    uuid_generate(uuid);
    return *uuid;
}
