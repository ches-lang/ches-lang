#pragma once

#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <string>
#include <vector>
#include "parser.cpp"
#include "syntax.cpp"

#define LN(i)       (bytecode.at(i))
#define TK(i, j)    (bytecode.at(i).at(j))
#define CH(i, j, k) (bytecode.at(i).at(j).at(k))



typedef struct FuncData_ {

    std::vector<unsigned char> id;
    std::vector<unsigned char> name;
    std::vector<std::vector<std::vector<unsigned char>>> source;
    int start;
    int end;

    FuncData_() {}

    FuncData_(std::vector<unsigned char> i, std::vector<unsigned char> nm) {
        id = i;
        name = nm;
    }

    FuncData_(std::vector<unsigned char> i, std::vector<unsigned char> nm, int st, int ed) {
        id = i;
        name = nm;
        st = start;
        ed = end;
    }

    static FuncData_ findById(std::vector<FuncData_> fd, std::vector<unsigned char> id) {
        for(FuncData_ f : fd)
            if(f.id == id)
                return f;
        return FuncData_();
    }

    static FuncData_ findByName(std::vector<FuncData_> fd, std::vector<unsigned char> nm) {
        for(FuncData_ f : fd)
            if(f.name == nm)
                return f;
        return FuncData_();
    }

} FuncData;



//relpath
class Bytecode {

public:

    std::vector<unsigned char> source;

    Bytecode() {}

    Bytecode(std::vector<unsigned char> src) {
        source = src;
    }

    Bytecode(int src) { //
        source.push_back((unsigned char)src);
    }

    Bytecode(std::string src) {
        for(unsigned char s : src)
            source.push_back(s);
    }

    Bytecode(Node tree) {
        source = toBytecode(tree).source;
    }

    Bytecode(std::vector<Bytecode> src) {
        for(Bytecode bc : src) {
            append(Bytecode("compiled_ches"));
            append(Bytecode(std::vector<unsigned char> { SI_LNDIV, SI_BCSTART, SI_LNDIV }));
            append(Bytecode(bc));
        }
    }

    Bytecode(std::vector<std::vector<std::vector<unsigned char>>> src) {
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

    inline Bytecode append(unsigned char src) {
        source.push_back(src);
        return source;
    }

    inline Bytecode append(Bytecode src) {
        for(unsigned char uc : src.source)
            source.push_back(uc);

        return source;
    }

    inline Bytecode pop() {
        source.pop_back();
        return source;
    }

    inline int toInt() {
        return *(unsigned int *)&source;
    }

    inline std::string toString() {
        std::string res;
        for(unsigned char c : source)
            res.push_back(c);
        return res;
    }

    // vector型の命令を取得
    std::vector<std::vector<std::vector<unsigned char>>> divide() {
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

private:

    int index = 0;
    std::vector<std::vector<std::vector<unsigned char>>> lines;
    std::vector<FuncData> funcdata;
    std::string spaceName;
    std::string className;
    int lslen = 0;
    int lllen = 0;

    Bytecode toBytecode(Node tree) {
        lines.push_back({ { 0x63, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x64, 0x5f, 0x63, 0x68, 0x65, 0x73 } });

        for(Node nd : tree.children) {
            if(nd.type == N_DEFFUNC) {
                funcdata.push_back(FuncData(Bytecode(getRandom(10)).source, Bytecode(nd.getToken(0).string).source));
            }
        }

        while(index < tree.children.size()) {
            toBytecode_scan(tree.getNode(index));
            index++;
        }

        return Bytecode(lines);
    }

    void toBytecode_scan(Node node) {
        try {

            switch(node.type) {
                case N_CALLFUNC: {
                    std::vector<unsigned char> funcname = Bytecode(node.getToken(0).string).source;
                    lines.push_back({ { I_CALL }, FuncData::findByName(funcdata, funcname).id });
                } break;

                case N_DEFFUNC: {
                    std::vector<unsigned char> funcname = Bytecode(node.getToken(0).string).source;
                    std::vector<unsigned char> funcid = Bytecode(FuncData::findByName(funcdata, funcname).id).source;
                    lines.push_back({ { I_GROUP }, funcid, funcname });
                    if(node.children.size() > 1)
                        lllen += node.getNode(0).children.size();
                } break;

                case N_DEFVAR: {
                    lines.push_back({ { I_LSPUSH }, {} });
                    lslen++;
                } break;

                case N_INITVAR: {
                    Token token = node.getToken(2);
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

    std::vector<unsigned char> getRandom(int num) {
        std::vector<unsigned char> res;
        std::random_device rnd;
        for(int i = 0; i < num; i++)
            res.push_back(rnd() % std::numeric_limits<unsigned char>::max());
        return res;
    }
};
