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
        append(Bytecode(IT_LineDiv));
        append(Bytecode(bc));
    }
}

Bytecode::Bytecode(LineSeq source) {
    for(int ln = 0; ln < source.size(); ln++) {
        for(int tk = 0; tk < source[ln].size(); tk++) {
            for(int ch = 0; ch < source[ln][tk].size(); ch++) {
                if(source[ln][tk][ch] == IT_LineDiv) this->append(IT_LineDiv);
                else if(source[ln][tk][ch] == IT_TokenDiv) this->append(IT_TokenDiv);
                this->append(source[ln][tk][ch]);
            }

            if(tk != source[ln].size() - 1) this->append(IT_TokenDiv);
        }

        if(ln != source.size() - 1) this->append(IT_LineDiv);
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
            case IT_LineDiv:
            if(i + 1 < this->source.size() && this->source[i + 1] == IT_LineDiv) {
                res.back().back().push_back(IT_LineDiv);
                i++;
            } else {
                res.push_back({{}});
            }
            break;

            case IT_TokenDiv:
            if(i + 1 < this->source.size() && this->source[i + 1] == IT_TokenDiv) {
                res.back().back().push_back(IT_TokenDiv);
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
        if(node.type == ND_DefFunc)
            funcdata.push_back(FuncData(Bytecode(this->generateUUID()).source, Bytecode(node.tokenAt(0).string).source));

    int index = 0;
    LineSeq lineSeq = this->toLineSeq(tree, index);
    std::copy(lineSeq.begin(), lineSeq.end(), std::back_inserter(this->lines));
    std::cout << std::endl;

    return Bytecode(lines);
}

// ある階層内のすべての子ノードを調べます
/*void Bytecode::scanNode(Node node) {
    for(int i = 0; i < node.children.size(); i++) {
        LineSeq resLines = this->toLineSeq(node, i);
        //std::cout << "bc: "; for(TokenSeq ts : resLines) for(ByteSeq bs : ts) for(Byte b : bs) std::cout << (int)b << " "; ; std::cout << std::endl;
        std::copy(resLines.begin(), resLines.end(), std::back_inserter(this->lines));
    }
}*/

// ノードを調べてバイトコードに変換し、LineSeq型の行列を返します
// 基本的に scanNode(Node) により呼ばれます
LineSeq Bytecode::toLineSeq(Node parentNode, int &index) {
    Node node = parentNode.childAt(index);
    std::cout << std::endl << index << " > "<< parentNode.children.size() << " | " << (int)node.type << std::endl;
    LineSeq result;

    try {

        switch(node.type) {
            case ND_Unknown: {std::cout<<"unknown"<<std::endl;
                result.push_back({ { IT_Unknown } });
            } break;

            case ND_Root: {std::cout<<"root"<<std::endl;
                for(int i = 0; i < node.children.size(); i++) {
                    LineSeq resLines = this->toLineSeq(node, i);
                    //std::cout << "bc: "; for(TokenSeq ts : resLines) for(ByteSeq bs : ts) for(Byte b : bs) std::cout << (int)b << " "; ; std::cout << std::endl;
                    std::copy(resLines.begin(), resLines.end(), std::back_inserter(result));
                }
            } break;

            case ND_DefVar: {std::cout<<"defvar"<<std::endl;
                result.push_back({ { IT_LSPush }, {} });
                int i = 1;
                this->toLineSeq(node, i);
                lslen++;
            } break;

            case ND_InitVar: {std::cout<<"initvar"<<std::endl;
                Token token = node.tokenAt(2);
                ByteSeq value;

                if(token.type == TK_Number)
                    value = Bytecode(std::stoi(token.string)).source;
                else if(token.type == TK_String)
                    value = Bytecode(token.string).source;

                result.push_back({ { IT_LSPush }, value });
                lslen++;
            } break;

            case ND_DefFunc: {std::cout<<"deffunc"<<std::endl;
                ByteSeq funcname = Bytecode(node.tokenAt(0).string).source;
                ByteSeq funcid = Bytecode(FuncData::findByName(funcdata, funcname).id).source;
                result.push_back({ { IT_Label }, funcid, funcname });

                lllen += node.childAt(0).children.size();

                int i = 2;
                this->toLineSeq(node, i);
            } break;

            case ND_CallFunc: {std::cout<<"callfunc"<<std::endl;
                ByteSeq funcname = Bytecode(node.tokenAt(0).string).source;
                result.push_back({ { IT_Jump }, FuncData::findByName(funcdata, funcname).id });
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

                //LineSeq condLines = this->toLineSeq(node.childAt(0), index);
                //for(Node n : node.childAt(0).children) std::cout << (int)n.type << " "; std::cout << std::endl;

                int i = 1;
                this->toLineSeq(node, i);

                // ROOTノードの階層をチェックし、resultに追加していく
                /*LineSeq procLines;

                for(int i = 0; i < node.childAt(1).children.size(); i++) {
                    LineSeq resLines = this->toLineSeq(node.childAt(1).childAt(i), index);
                    std::copy(resLines.begin(), resLines.end(), std::back_inserter(procLines));
                }*/

                // procLinesなどのサイズも考慮してジャンプ先を出す
                result.push_back({ { IT_IFJump }, {} });

                // condLinesとprocLinesをresultに結合
                //std::copy(condLines.begin(), condLines.end(), std::back_inserter(result));
                //std::copy(procLines.begin(), procLines.end(), std::back_inserter(result));

            } break;

            case ND_Else: {std::cout<<"else"<<std::endl;
                
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
