#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "console.cpp"
#include "parser.cpp"



#define ENDOFFILE  -1
#define UNKNOWN     0
#define INDENT      1
#define NEWLINE     2
#define COMMENTOUT  3
#define KEYWORD     4
#define IDENTIFIER  5
#define NUMBER      6
#define CHARACTER   7
#define STRING      8
#define EXCLAMATION 9
#define QUESTION    10
#define TILDE       11
#define PLUS        12
#define HYPHEN      13
#define ASTERISK    14
#define SLASH       15
#define PERCENTAGE  16
#define CARET       17
#define EQUAL       18
#define PIPE        19
#define AMPERSAND   20
#define PERIOD      21
#define COMMA       22
#define COLON       23
#define SEMICOLON   24
#define LPAREN      25
#define RPAREN      26
#define LBRACK      27
#define RBLACK      28
#define LANGBLACK   29
#define RANGBLACK   30
#define LBRACE      31
#define RBRACE      32

#define ROOT        33
#define DEFFUNC     34
#define CALLFUNC    35
#define ARGS        36



class Converter {

public:

    Node tree;
    std::string path;
    std::vector<std::vector<std::vector<unsigned char>>> res;

    Converter() {}

    Converter(Node t, std::string p) {
        tree = t;
        path = p;
    }

    std::vector<std::vector<std::vector<unsigned char>>> run() {
        bool b;
        res.push_back({{0x43, 0x4F, 0x4D, 0x50, 0x49, 0x4C, 0x45, 0x44, 0x5F, 0x43, 0x48, 0x45, 0x53}});
        do {
            b = scan();
        } while(b);
        //out();
        return res;
    }

    void out() {
        for(int i = 0; i < res.size(); i++) {
            for(int j = 0; j < res[i].size(); j++) {
                for(int k = 0; k < res[i][j].size(); k++) {
                    std::cout << (int)res[i][j][k] << " ";
                } std::cout << " ";
            } std::cout << std::endl;
        }
    }

private:

    int index = -1;

    bool scan() {
        index++;
        if(index > tree.nodes.size()) return false;
        Node node = tree.nodes[index];

        if(node.type == CALLFUNC) {
            std::vector<std::vector<unsigned char>> arr;
            arr.push_back({0x81, 0x9D});
            arr.push_back(toChars(node.children[0].token.second));
            for(Child c : node.nodes[0].children)
                arr.push_back(toChars(c.token.second));
            res.push_back(arr);
        }

        else if(node.type == DEFFUNC) {
            std::vector<std::vector<unsigned char>> arr;
            arr.push_back({0x4E, 0xC0});
            arr.push_back(toChars(node.children[0].token.second));
            for(Child c : node.nodes[0].children)
                arr.push_back(toChars(c.token.second));
            res.push_back(arr);
        }

        return true;
    }

    std::vector<unsigned char> toChars(std::string str) {
        std::vector<unsigned char> res;
        for(int i = 0; i < str.length(); i++)
            res.push_back(str[i]);
        return res;
    }
};
