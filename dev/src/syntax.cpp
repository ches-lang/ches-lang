#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "console.cpp"



/* ! ? ~ + - * / % ^ = | & . , : ; ( ) [ ] < > { } */
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



            /*

            // ↓のソースコードを...

            main(str[] args)
              println("Hello, world")

            // ↓に変換！

            N ROOT
              N DEFFUNC
                C main
                N ARGS
                  C str
                  C [
                  C ]
                  C args
              N CALLFUNC
                C println
                N ARGS
                  C Hello, World

            */



struct Child {

    std::pair<char, std::string> token;

    Child() {}

    Child(std::pair<char, std::string> tk) {
        token = tk;
    }
};



struct Node {

    std::vector<Node> nodes;
    std::vector<Child> children;

    char type;

    Node() {}

    Node(char t) {
        type = t;
    }

    inline void add(Node n) {
        nodes.push_back(n);
    }

    inline void add(Child c) {
        children.push_back(c);
    }

    inline void set(std::vector<Node> n) {
        nodes = n;
    }

    inline void set(std::vector<Child> c) {
        children = c;
    }

    void out(std::string head) {
        for(int i = 0; i < children.size(); i++)
            std::cout << head << (int)type << " " << (int)children[i].token.first << " " << children[i].token.second << std::endl;
        for(int i = 0; i < nodes.size(); i++)
            nodes[i].out(head += "  ");
    }
};
