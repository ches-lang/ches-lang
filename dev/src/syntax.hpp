#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "console.cpp"
#include "parser.cpp"



/* ! ? ~ + - * / % ^ = | & . , : ; ( ) [ ] < > { } */
#define ENDOFFILE   0
#define UNKNOWN     1
#define INDENT      2
#define NEWLINE     3
#define COMMENTOUT  4
#define KEYWORD     5
#define IDENTIFIER  6
#define NUMBER      7
#define CHARACTER   8
#define STRING      9
#define EXCLAMATION 10
#define QUESTION    11
#define TILDE       12
#define PLUS        13
#define HYPHEN      14
#define ASTERISK    15
#define SLASH       16
#define PERCENTAGE  17
#define CARET       18
#define EQUAL       19
#define PIPE        20
#define AMPERSAND   21
#define PERIOD      22
#define COMMA       23
#define COLON       24
#define SEMICOLON   25
#define LPAREN      26
#define RPAREN      27
#define LBRACK      28
#define RBRACK      29
#define LANGBRACK   30
#define RANGBRACK   31
#define LBRACE      33
#define RBRACE      34

#define N_UNKNOWN   0x00
#define N_ROOT      0x01
#define N_TOKEN     0x02
#define N_DEFVAR    0x03
#define N_INITVAR   0x04
#define N_FREEVAR   0x05
#define N_DEFFUNC   0x06
#define N_CALLFUNC  0x07
#define N_ARGS      0x08
#define N_IF        0x09
#define N_LOOP      0x10
#define N_COUNT     0x11
#define N_LOGIC     0x12
#define N_COMP      0x13
#define N_EXPRESS   0x14
#define N_ITEM      0x15
#define N_OPE       0x16
#define N_EQUAL     0x17
#define N_LESS      0x18
#define N_GREATER   0x19
#define N_LESSEQL   0x20
#define N_GRTREQL   0x21

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
#define I_IFJUMP    0x16
#define I_GOTO      0x17



struct Token {

    int index = 0;
    unsigned char type = UNKNOWN;
    std::string string = "";

    Token();

    Token(unsigned char type);

    Token(std::string string);

    Token(unsigned char type, std::string string);

    Token(unsigned char type, std::string string, int index);

    bool compare(Token token);

    Token getCloseParen();

    Token getOpenParen();

    std::pair<int, int> getPosition(std::string source);

    static std::pair<int, int> getPosition(std::string source, int index) {
        int line = 0;
        int pos = 0;

        for(int i = 0; i < index; i++) {
            if(source[i] == '\n') {
                pos = 0;
                line++;
            } else {
                pos++;
            }
        }

        return { line, pos };
    }

    static std::string getPositionText(std::string filePath, std::string source, int index) {
        std::pair pos = Token::getPosition(source, index);
        return filePath + ":" + std::to_string(pos.first + 1) + ":" + std::to_string(pos.second + 1);
    }

    std::string getPositionText(std::string filePath, std::string source);

    bool match(std::vector<unsigned char> matches);

    bool match(std::string regexp);
};



struct Node {

    unsigned char type = N_UNKNOWN;
    std::vector<Node> children;
    std::vector<Token> tokens;

    Node();

    Node(unsigned char type);

    Node(unsigned char type, std::vector<Node> children);

    Node(unsigned char type, std::vector<Token> tokens);

    Node(unsigned char type, std::vector<Node> children, std::vector<Token> tokens);

    void addChild(Node node);

    void addToken(Token token);

    Node childAt(int index);

    Token tokenAt(int index);

    void print();

    void print(std::string level);
};
