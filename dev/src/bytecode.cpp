#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "console.cpp"
#include "parser.cpp"

typedef std::vector<std::vector<std::vector<unsigned char>>> bytecode;
typedef std::vector<unsigned char> bytecode_serial;



class Bytecode {

public:

    Node tree;

    Bytecode() {}

    Bytecode(Node node) {
        tree = node;
    }

    bytecode run() {
        bytecode res;
        bytecode scanned;

        while(!eof) {
            scanned = scan(node);
            res.insert(res.end(), scanned.begin(), scanned.end());
        }

        return res;
    }

private:

    bool eof = false;
    int index = -1;

    bytecode scan(Node node) {
        bytecode res;
        return res;
    }

    std::vector<std::vector<std::vector<unsigned char>>> getLine() {
        index++;
        Node nd = node.getNode(index);

        if(index == nd.children.size() - 1) eof = true;

        try {
            return scan(nd);
        } catch(std::out_of_range ignored) {
            std::cout << "EXCEPTION" << std::endl;
            return std::vector<std::vector<unsigned char>>();
        }
    }

    std::vector<std::vector<unsigned char>> scan(Node nd) {
        std::vector<std::vector<unsigned char>> res;

        std::string type = nd.type;
        std::vector<Token> tokens = nd.tokens;
        std::vector<Node> nodes = nd.children;

        int tokenlen = tokens.size();
        int nodelen = nodes.size();

        if(type == "DEFFUNC") {
            res.push_back({ 0x00, 0x01 });
            if(nodelen == 1) {
                res
            }
        }

        return res;
    }

    std::vector<unsigned char> getBytecode(std::vector<std::vector<std::vector<unsigned char>>> src) {
        std::vector<unsigned char> res;
        for(int line_i = 0; line_i < src.size(); line_i++) {
            for(int token_i = 0; token_i < src[line_i].size(); token_i++) {
                for(int chr_i = 0; chr_i < src[line_i][token_i].size(); chr_i++) {
                    res.push_back(src[line_i][token_i][chr_i]);
                }
            }
        }
        return res;
    }
};
