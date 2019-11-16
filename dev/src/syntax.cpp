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



struct Token {

    char type = UNKNOWN;
    std::string string = "";

    Token() {}

    Token(char tp) {
        type = tp;
    }

    Token(std::string st) {
        string = st;
    }

    Token(char tp, std::string st) {
        type = tp;
        string = st;
    }

    bool compare(Token tk) {
        return (type == tk.type && string == tk.string);
    }
};



struct Node {

    std::string type = "";
    std::vector<Node> children;
    std::vector<Token> tokens;

    Node() {}

    Node(std::string tp) {
        type = tp;
    }

    Node(std::string tp, std::vector<Node> ch) {
        type = tp;
        children = ch;
    }

    Node(std::string tp, std::vector<Token> tk) {
        type = tp;
        tokens = tk;
    }

    Node(std::string tp, std::vector<Node> ch, std::vector<Token> tk) {
        type = tp;
        children = ch;
        tokens = tk;
    }

    void addNode(Node nd) {
        children.push_back(nd);
    }

    void addToken(Token tk) {
        tokens.push_back(tk);
    }

    Node getNode(int index) {
        return children[index];
    }

    Token getToken(int index) {
        return tokens[index];
    }

    bool isEmpty() {
        return (type == "");
    }

    void out(std::string level) {
        std::cout << "\033[30m" << level << "\033[m" << type << std::endl;
        for(Token t : tokens)
            std::cout << "\033[30m" << level << "||" << "\033[m" << ((t.string == "|") ? "[pipe]" : t.string) << std::endl;
        for(Node n : children)
            n.out(level + "||");
    }
};
