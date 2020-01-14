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

Bytecode::Bytecode(std::vector<unsigned char> src) {
    source = src;
}

Bytecode::Bytecode(int src) { //
    source.push_back((unsigned char)src);
}

Bytecode::Bytecode(std::string src) {
    for(unsigned char s : src)
        source.push_back(s);
}

Bytecode::Bytecode(Node tree) {
    source = toBytecode(tree).source;
}

Bytecode::Bytecode(std::vector<Bytecode> src) {
    for(Bytecode bc : src) {
        append(Bytecode("compiled_ches"));
        append(Bytecode(std::vector<unsigned char> { SI_LNDIV, SI_BCSTART, SI_LNDIV }));
        append(Bytecode(bc));
    }
}

Bytecode::Bytecode(std::vector<std::vector<std::vector<unsigned char>>> src) {
    for(int ln = 0; ln < src.size(); ln++) {
        for(int tk = 0; tk < src[ln].size(); tk++) {
            for(int ch = 0; ch < src[ln][tk].size(); ch++) {
                if(src[ln][tk][ch] == SI_LNDIV) append(SI_LNDIV);
                else if(src[ln][tk][ch] == SI_TKDIV) append(SI_TKDIV);
                append(src[ln][tk][ch]);
            }
            if(tk != src[ln].size() - 1) append(SI_TKDIV);
        }
        if(ln != src.size() - 1) append(SI_LNDIV);
    }
}

Bytecode Bytecode::append(unsigned char src) {
    source.push_back(src);
    return source;
}

Bytecode Bytecode::append(Bytecode src) {
    for(unsigned char uc : src.source)
        source.push_back(uc);
    return source;
}

Bytecode Bytecode::pop() {
    source.pop_back();
    return source;
}

int Bytecode::toInt() {
    return *(unsigned int *)&source;
}

std::string Bytecode::toString() {
    std::string res;
    for(unsigned char c : source)
        res.push_back(c);
    return res;
}

// vector型の命令を取得
std::vector<std::vector<std::vector<unsigned char>>> Bytecode::divide() {
    std::vector<std::vector<std::vector<unsigned char>>> res = {{{}}};

    for(int i = 0; i < source.size(); i++) {
        switch(source[i]) {
            case SI_LNDIV:
            if(i + 1 < source.size() && source[i + 1] == SI_LNDIV) {
                res.back().back().push_back(SI_LNDIV);
                i++;
            } else {
                res.push_back({{}});
            }
            break;

            case SI_TKDIV:
            if(i + 1 < source.size() && source[i + 1] == SI_TKDIV) {
                res.back().back().push_back(SI_TKDIV);
                i++;
            } else {
                res.back().push_back({});
            }
            break;

            default:
            res.back().back().push_back(source[i]);
            break;
        }
    }

    return res;
}

Bytecode Bytecode::toBytecode(Node tree) {
    lines.push_back({ { 0x63, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x64, 0x5f, 0x63, 0x68, 0x65, 0x73 } });

    for(Node nd : tree.children) {
        if(nd.type == N_DEFFUNC) {
            funcdata.push_back(FuncData(Bytecode(getRandom(10)).source, Bytecode(nd.tokenAt(0).string).source));
        }
    }

    while(index < tree.children.size()) {
        toBytecode_scan(tree.childAt(index));
        index++;
    }

    return Bytecode(lines);
}

void Bytecode::toBytecode_scan(Node node) {
    try {

        switch(node.type) {
            case N_CALLFUNC: {
                std::vector<unsigned char> funcname = Bytecode(node.tokenAt(0).string).source;
                lines.push_back({ { I_CALL }, FuncData::findByName(funcdata, funcname).id });
            } break;

            case N_DEFFUNC: {
                std::vector<unsigned char> funcname = Bytecode(node.tokenAt(0).string).source;
                std::vector<unsigned char> funcid = Bytecode(FuncData::findByName(funcdata, funcname).id).source;
                lines.push_back({ { I_GROUP }, funcid, funcname });
                if(node.children.size() > 1)
                    lllen += node.childAt(0).children.size();
            } break;

            case N_DEFVAR: {
                lines.push_back({ { I_LSPUSH }, {} });
                lslen++;
            } break;

            case N_IF: {/*
                    for(; index < node.children.size(); index++)
                    lines.push_back(node.childAt(index));
                index--;
                lines.push_back({ { I_IFJUMP }, {  } });*/
            } break;

            case N_INITVAR: {
                Token token = node.tokenAt(2);
                std::vector<unsigned char> value;
                if(token.type == NUMBER)
                    value = Bytecode(std::stoi(token.string)).source;
                else if(token.type == STRING)
                    value = Bytecode(token.string).source;
                lines.push_back({ { I_LSPUSH }, value });
                lslen++;
            } break;
        }

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

std::vector<unsigned char> Bytecode::getRandom(int num) {
    std::vector<unsigned char> res;
    std::random_device rnd;
    for(int i = 0; i < num; i++)
        res.push_back(rnd() % std::numeric_limits<unsigned char>::max());
    return res;
}
