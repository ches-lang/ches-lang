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

#define N_UNKNOWN   0x00
#define N_ROOT      0x01
#define N_DEFVAR    0x02
#define N_INITVAR   0x03
#define N_FREEVAR   0x04
#define N_DEFFUNC   0x05
#define N_CALLFUNC  0x06
#define N_ARGS      0x07
#define N_LOOP      0x08
#define N_COUNT     0x09
#define N_LOGIC     0x10
#define N_COMP      0x11
#define N_EXPRESS   0x12
#define N_ITEM      0x13
#define N_OPE       0x14
#define N_EQUAL     0x15
#define N_LANGBLACK 0x16
#define N_RANGBLACK 0x17

#define SI_BCSTART  0x00
#define SI_LNDIV    0x01
#define SI_TKDIV    0x02
#define SI_LLPREF   0x03

#define I_UNKNOWN   0x04
#define I_GROUP     0x05
#define I_LSPUSH    0x06
#define I_LLPUSH    0x07
#define I_CALL      0x08
#define I_RETURN    0x09
#define I_LAND      0x10
#define I_LOR       0x11
#define I_NADD      0x12
#define I_NSUB      0x13
#define I_NMUL      0x14
#define I_NDIV      0x15



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

    unsigned char type = N_UNKNOWN;
    std::vector<Node> children;
    std::vector<Token> tokens;

    Node() {}

    Node(unsigned char tp) {
        type = tp;
    }

    Node(unsigned char tp, std::vector<Node> ch) {
        type = tp;
        children = ch;
    }

    Node(unsigned char tp, std::vector<Token> tk) {
        type = tp;
        tokens = tk;
    }

    Node(unsigned char tp, std::vector<Node> ch, std::vector<Token> tk) {
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
        return (type == N_UNKNOWN);
    }

    void out(std::string level) {
        std::cout << "\033[30m" << level << "\033[m" << (int)type << std::endl;
        for(Token t : tokens)
            std::cout << "\033[30m" << level << "||" << "\033[m" << ((t.string == "|") ? "[pipe]" : t.string) << std::endl;
        for(Node n : children)
            n.out(level + "||");
    }
};
