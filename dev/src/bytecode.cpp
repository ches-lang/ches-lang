#pragma once

#include "bytecode.hpp"



FuncData_::FuncData_() {}

FuncData_::FuncData_(ByteSeq i, ByteSeq nm) {
    id = i;
    name = nm;
}

FuncData_::FuncData_(ByteSeq i, ByteSeq nm, int st, int ed) {
    id = i;
    name = nm;
    st = start;
    ed = end;
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
    this->source = this->toBytecode(tree).source;
}

Bytecode::Bytecode(std::vector<Bytecode> source) {
    for(Bytecode bc : source) {
        append(Bytecode("compiled_ches"));
        //append(Bytecode(ByteSeq { IT_LineDivide, IT_BytecodeStart, IT_LineDivide }));
        append(Bytecode(IT_LineDivide));
        append(Bytecode(bc));
    }
}

Bytecode::Bytecode(LineSeq source) {
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
    // マジックナンバーを追加
    this->lines.push_back({ MAGIC_NUMBER });

    for(Node node : tree.children)
        if(node.type == ND_DefFunction)
            funcdata.push_back(FuncData(Bytecode(this->generateUUID()).source, Bytecode(node.tokenAt(0).string).source));

    this->scanNode(tree);

    return Bytecode(lines);
}

// ある階層内のすべての子ノードを調べます
void Bytecode::scanNode(Node node) {
    for(int i = 0; i < node.children.size(); i++) {
        LineSeq resLines = this->nodeToBytecode(node.children[i], i);
        //std::cout << "bc: "; for(TokenSeq ts : resLines) for(ByteSeq bs : ts) for(Byte b : bs) std::cout << (int)b << " "; ; std::cout << std::endl;
        std::copy(resLines.begin(), resLines.end(), std::back_inserter(this->lines));
    }
}

// ノードを調べてバイトコードに変換し、LineSeq型の行列を返します
// 基本的に scanNode(Node) により呼ばれます
LineSeq Bytecode::nodeToBytecode(Node node, int &index) {std::cout<<"index: "<<index<<std::endl;
    LineSeq result;

    try {

        switch(node.type) {
            case ND_Unknown: {
                result.push_back({ { IT_Unknown } });
            } break;

            case ND_DefVariable: {std::cout<<"deffunc"<<std::endl;
                result.push_back({ { IT_LocalStackPush }, {} });
                this->scanNode(node.childAt(1));
                lslen++;
            } break;

            case ND_InitVariable: {std::cout<<"initvar"<<std::endl;
                Token token = node.tokenAt(2);
                ByteSeq value;

                if(token.type == TK_Number)
                    value = Bytecode(std::stoi(token.string)).source;
                else if(token.type == TK_String)
                    value = Bytecode(token.string).source;

                result.push_back({ { IT_LocalStackPush }, value });
                lslen++;
            } break;

            case ND_DefFunction: {std::cout<<"deffunc"<<std::endl;
                ByteSeq funcname = Bytecode(node.tokenAt(0).string).source;
                ByteSeq funcid = Bytecode(FuncData::findByName(funcdata, funcname).id).source;
                result.push_back({ { IT_Label }, funcid, funcname });

                lllen += node.childAt(0).children.size();

                // ROOTノードの階層をチェックし、resultに追加していく
                for(int i = 0; i < node.childAt(1).children.size(); i++) {
                    LineSeq resLines = this->nodeToBytecode(node.childAt(1).childAt(i), index);
                    std::copy(resLines.begin(), resLines.end(), std::back_inserter(result));
                }
            } break;

            case ND_CallFunction: {std::cout<<"callfunc"<<std::endl;
                ByteSeq funcname = Bytecode(node.tokenAt(0).string).source;
                result.push_back({ { IT_Jump }, FuncData::findByName(funcdata, funcname).id });
            } break;

            case ND_If: {std::cout<<"if"<<std::endl;
                // 次のノードに else / elseif がくれば

                std::cout << index << std::endl;

                if(node.children.size() < index) {
                    Byte nextNodeType = node.childAt(index + 1).type;

                    if(nextNodeType == ND_ElseIf || nextNodeType == ND_Else) {
                        //
                    }
                }

                // 条件式をチェック

                LineSeq condLines = this->nodeToBytecode(node.childAt(0), index);
                for(Node n : node.childAt(0).children) std::cout << (int)n.type << " "; std::cout << std::endl;

                // ROOTノードの階層をチェックし、resultに追加していく
                LineSeq procLines;

                for(int i = 0; i < node.childAt(1).children.size(); i++) {
                    LineSeq resLines = this->nodeToBytecode(node.childAt(1).childAt(i), index);
                    std::copy(resLines.begin(), resLines.end(), std::back_inserter(procLines));
                }

                // procLinesなどのサイズも考慮してジャンプ先を出す
                result.push_back({ { IT_IFJump }, {} });

                // condLinesとprocLinesをresultに結合
                std::copy(condLines.begin(), condLines.end(), std::back_inserter(result));
                std::copy(procLines.begin(), procLines.end(), std::back_inserter(result));

            } break;
        }

        return result;

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
        return result;
    }
}

Byte Bytecode::generateUUID() {
    uuid_t uuid;
    uuid_generate(uuid);
    return *uuid;
}
