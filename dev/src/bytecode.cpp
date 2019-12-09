#pragma once

#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <string>
#include <vector>
#include "console.cpp"
#include "parser.cpp"
#include "syntax.cpp"

#define LN(i)       (bytecode.at(i))
#define TK(i, j)    (bytecode.at(i).at(j))
#define CH(i, j, k) (bytecode.at(i).at(j).at(k))



typedef struct FuncData_ {

    std::vector<unsigned char> id;
    std::vector<unsigned char> name;
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

    static FuncData_ find(std::vector<FuncData_> fd, std::vector<unsigned char> nm) {
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

    Bytecode(int src) {
        source.push_back((unsigned char)src);
    }

    Bytecode(std::string src) {
        for(unsigned char s : src)
            source.push_back(s);
    }
    
    Bytecode(Node tree) {
        source = toBytecode(tree).source;
    }

    Bytecode(std::vector<std::vector<unsigned char>> src) {
        for(std::vector<unsigned char> bc : src) {
            for(unsigned char uc : bc)
                source.push_back(uc);
            source.push_back(SI_LNDIV);
        }
        source.pop_back();
    }

    Bytecode(std::vector<std::vector<Bytecode>> src) {
        for(int ln = 0; ln < src.size(); ln++) {
            for(int tk = 0; tk < src[ln].size(); tk++) {
                for(int ch = 0; ch < src[ln][tk].source.size(); ch++) {
                    if(src[ln][tk].source[ch] == SI_LNDIV) source.push_back(SI_LNDIV);
                    else if(src[ln][tk].source[ch] == SI_TKDIV) source.push_back(SI_TKDIV);
                    source.push_back(src[ln][tk].source[ch]);
                }
                if(tk < src[ln].size() - 1) source.push_back(SI_TKDIV);
            }
            if(ln < src.size() - 1) source.push_back(SI_LNDIV);
        }
    }

    Bytecode(std::vector<std::vector<std::vector<unsigned char>>> src) {
        for(int ln = 0; ln < src.size(); ln++) {
            for(int tk = 0; tk < src[ln].size(); tk++) {
                for(int ch = 0; ch < src[ln][tk].size(); ch++) {
                    if(src[ln][tk][ch] == SI_LNDIV) source.push_back(SI_LNDIV);
                    else if(src[ln][tk][ch] == SI_TKDIV) source.push_back(SI_TKDIV);
                    source.push_back(src[ln][tk][ch]);
                }
                if(tk < src[ln].size() - 1) source.push_back(SI_TKDIV);
            }
            if(ln < src.size() - 1) source.push_back(SI_LNDIV);
        }
    }

    int toInt() {
        return *(unsigned int *)&source;
    }

    std::string toString() {
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

        for(std::vector<std::vector<unsigned char>> a : res) {
            for(std::vector<unsigned char> b : a) {
                for(unsigned char c : b) {
                    std::cout << (int)c << " ";
                }std::cout << "| ";
            }std::cout << std::endl;
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
                case N_DEFFUNC: {
                    std::vector<unsigned char> funcname = Bytecode(node.getToken(0).string).source;
                    std::vector<unsigned char> funcid = Bytecode(FuncData::find(funcdata, funcname).id).source;
                    lines.push_back({ { I_GROUP }, funcid, funcname });
                    if(node.children.size() > 1)
                        lllen += node.getNode(0).children.size();
                } break;

                case N_CALLFUNC: {
                    std::vector<unsigned char> funcname = Bytecode(node.getToken(0).string).source;
                    lines.push_back({ { I_CALL }, FuncData::find(funcdata, funcname).id });
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
