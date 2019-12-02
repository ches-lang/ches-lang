#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "console.cpp"
#include "parser.cpp"
#include "syntax.cpp"



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

    Bytecode(std::vector<std::vector<Bytecode>> src) {
        for(int ln = 0; ln < src.size(); ln++) {
            for(int tk = 0; tk < src[ln].size(); tk++) {
                for(int ch = 0; ch < src[ln][tk].source.size(); ch++) {
                    if(src[ln][tk].source[ch] == N_LINEDIV) source.push_back(N_LINEDIV);
                    else if(src[ln][tk].source[ch] == N_TOKENDIV) source.push_back(N_TOKENDIV);
                    source.push_back(src[ln][tk].source[ch]);
                }
                if(tk < src[ln].size() - 1) source.push_back(N_TOKENDIV);
            }
            if(ln < src.size() - 1) source.push_back(N_LINEDIV);
        }
    }

    Bytecode append(Bytecode src) {
        for(unsigned char c : src.source)
            source.push_back(c);
        return source;
    }

    Bytecode append(unsigned char src) {
        source.push_back(src);
        return source;
    }

    Bytecode append(std::vector<unsigned char> src) {
        for(unsigned char c : src)
            source.push_back(c);
        return source;
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

    std::vector<std::vector<Bytecode>> divide() {
        std::vector<std::vector<Bytecode>> res;
        std::vector<Bytecode> ln;
        Bytecode tk;

        for(int i = 0; i < source.size(); i++) {
            if(source[i] == N_LINEDIV) {
                if(i - 1 < source.size() && source[i + 1] == N_LINEDIV) {
                    tk.source.push_back(source[i]);
                    i++;
                } else {
                    ln.push_back(tk);
                    tk.source.clear();
                    res.push_back(ln);
                    ln.clear();
                }
            } else if(source[i] == N_TOKENDIV) {
                if(i - 1 < source.size() && source[i + 1] == N_TOKENDIV) {
                    tk.source.push_back(source[i]);
                    i++;
                } else {
                    ln.push_back(tk);
                    tk.source.clear();
                }
            } else {
                tk.source.push_back(source[i]);
            }
        }

        ln.push_back(tk);
        res.push_back(ln);

        for(std::vector<Bytecode> a : res) {
            for(Bytecode b : a) {
                for(unsigned char c : b.source) {
                    std::cout << (int)c << " ";
                }std::cout << "| ";
            }std::cout << std::endl;
        }

        return res;
    }

private:

    int index = 0;
    std::vector<unsigned char> bytecode;
    std::string spaceName;
    std::string className;
    int lslen = 0;
    int lllen = 0;

    Bytecode toBytecode(Node tree) {
        std::vector<unsigned char> mn = { 0x63, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x64, 0x5f, 0x63, 0x68, 0x65, 0x73 };
        _append(mn);
        _append(N_LINEDIV);

        while(index < tree.children.size()) {
            toBytecode_scan(tree.getNode(index));
            index++;
        }

        return Bytecode(bytecode);
    }

    void toBytecode_scan(Node node) {
        try {

            if(node.type == N_DEFFUNC) {
                _append(I_GROUP);std::cout<<node.getToken(0).string;
                _append(Bytecode(node.getToken(0).string));
                _append(N_LINEDIV);

                // args
                lllen += node.getNode(0).children.size();
            } else if(node.type == N_CALLFUNC) {
                // args
                //for(int i = 0; i < node.getNode(0).children.size(); i++) {

                //}

                //line.push_back({ N_CALLFUNC });
                //line.push_back(getBytecode(node.getToken(0).string));
                //bc.push_back(line);
            }

        } catch(std::out_of_range ignored) {
            std::cout << "EXCEPTION" << std::endl;
        }
    }

    void _append(Bytecode src) {
        for(unsigned char c : src.source)
            bytecode.push_back(c);
    }

    void _append(unsigned char src) {
        bytecode.push_back(src);
    }

    void _append(std::vector<unsigned char> src) {
        for(unsigned char c : src)
            bytecode.push_back(c);
    }
};
