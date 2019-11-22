#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "console.cpp"
#include "parser.cpp"

typedef std::vector<std::vector<std::vector<unsigned char>>> bytecode_obj;
typedef std::vector<std::vector<unsigned char>> bytecode_ln;
typedef std::vector<unsigned char> bytecode;



class Bytecode {

public:

    Node tree;
    std::string spaceName;
    std::string className;

    Bytecode() {}

    Bytecode(Node node, std::string relpath) {
        tree = node;
        //
    }

    bytecode run() {
        bc.push_back({ { 0x63, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x64, 0x5f, 0x63, 0x68, 0x65, 0x73 } });
        while(index < tree.children.size()) {
            scan(tree.getNode(index));
            index++;
        }
        return getBytecode(bc);
    }

private:

    bytecode_obj bc;

    int index = 0;

    void scan(Node node) {
        try {

            bytecode_ln line;

            if(node.type == "DEFFUNC") {
                line.push_back({ 0xa2, 0x89 });
                line.push_back(getBytecode(node.getToken(0).string));
                bc.push_back(line);
                line.clear();
                for(int i = 0; i < node.getNode(0).children.size(); i++) {
                    line.push_back({ 0x14 });
                    line.push_back({ 0x9e, (unsigned char)i });
                    line.push_back({ 0x9e, 0xb6, (unsigned char)i });
                    bc.push_back(line);
                    line.clear();
                    line.push_back({ 0x4d });
                    line.push_back({ 0x9e, (unsigned char)i });
                    bc.push_back(line);
                    line.clear();
                }
            }

        } catch(std::out_of_range ignored) {
            std::cout << "EXCEPTION" << std::endl;
        }
    }

    bytecode mergeBytecode(bytecode bc_first, bytecode bc_second) {
        for(unsigned char uc : bc_second)
            bc_first.push_back(uc);
        return bc_first;
    }

    bytecode getBytecode(std::string str) {
        bytecode res;
        for(unsigned char s : str)
            res.push_back(s);
        return res;
    }

    bytecode getBytecode(bytecode_obj src) {
        bytecode res;//00を連続させて区別
        std::cout << std::endl;
        for(int ln_i = 0; ln_i < src.size(); ln_i++) {
            for(int tk_i = 0; tk_i < src[ln_i].size(); tk_i++) {
                for(int ch_i = 0; ch_i < src[ln_i][tk_i].size(); ch_i++) {
                    std::cout << src[ln_i][tk_i][ch_i];
                    res.push_back(src[ln_i][tk_i][ch_i]);
                    if(src[ln_i][tk_i][ch_i] == 0x00) res.push_back(0x00);
                    if(src[ln_i][tk_i][ch_i] == 0x01) res.push_back(0x01);
                }
                if(tk_i != src[ln_i].size() - 1) res.push_back(0x01);
            }
            if(ln_i != src.size() - 1) res.push_back(0x00);
            std::cout << std::endl;
        }
        std::cout << std::endl;
        return res;
    }
};
