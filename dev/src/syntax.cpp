#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "console.cpp"



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
#define N_DEFVAR    0x02
#define N_INITVAR   0x03
#define N_FREEVAR   0x04
#define N_DEFFUNC   0x05
#define N_CALLFUNC  0x06
#define N_ARGS      0x07
#define N_IF        0x08
#define N_LOOP      0x09
#define N_COUNT     0x10
#define N_LOGIC     0x11
#define N_COMP      0x12
#define N_EXPRESS   0x13
#define N_ITEM      0x14
#define N_OPE       0x15
#define N_EQUAL     0x16
#define N_LANGBRACK 0x17
#define N_RANGBRACK 0x18

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
#define I_IF        0x16
#define I_GOTO      0x17



struct Token {

    int index = 0;
    unsigned char type = UNKNOWN;
    std::string string = "";

    Token() {}

    Token(unsigned char type) {
        this->type = type;
    }

    Token(std::string string) {
        this->string = string;
    }

    Token(unsigned char type, std::string string) {
        this->type = type;
        this->string = string;
    }

    Token(unsigned char type, std::string string, int index) {
        this->type = type;
        this->string = string;
        this->index = index;
    }

    // Index won't be compared.
    inline bool compare(Token token) {
        return (this->type == token.type && this->string == token.string);
    }

    Token getCloseParen() {
        switch (this->type) {
            case LPAREN:
            return Token(RPAREN, ")", this->index);

            case LBRACK:
            return Token(RBRACK, "]", this->index);

            case LBRACE:
            return Token(RBRACE, "}", this->index);

            case LANGBRACK:
            return Token(RANGBRACK, ">", this->index);
        }

        return Token();
    }

    Token getOpenParen() {
        switch (this->type) {
            case RPAREN:
            return Token(LPAREN, "(", this->index);

            case RBRACK:
            return Token(LBRACK, "[", this->index);

            case RBRACE:
            return Token(LBRACE, "{", this->index);

            case RANGBRACK:
            return Token(LANGBRACK, "<", this->index);
        }

        return Token();
    }

    std::pair<int, int> getPosition(std::string source) {
        int line = 0;
        int pos = 0;

        for(int i = 0; i < this->index; i++) {
            if(source[i] == '\n') {
                pos = 0;
                line++;
            } else {
                pos++;
            }
        }

        return { line, pos };
    }

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

    std::string getPositionText(std::string filePath, std::string source) {
        std::pair pos = this->getPosition(source);
        return filePath + ":" + std::to_string(pos.first + 1) + ":" + std::to_string(pos.second + 1);
    }

    bool match(std::vector<unsigned char> matches) {
        bool res = false;

        for(int i = 0; i < matches.size(); i++) {
            if(matches[i] == this->type) {
                res = true;
            }
        }

        return res;
    }

    bool match(std::string regexp) {
        return std::regex_match(this->string, std::regex(regexp));
    }
};



struct Node {

    unsigned char type = N_UNKNOWN;
    std::vector<Node> children;
    std::vector<Token> tokens;

    Node() {}

    Node(unsigned char type) {
        this->type = type;
    }

    Node(unsigned char type, std::vector<Node> children) {
        this->type = type;
        this->children = children;
    }

    Node(unsigned char type, std::vector<Token> tokens) {
        this->type = type;
        this->tokens = tokens;
    }

    Node(unsigned char type, std::vector<Node> children, std::vector<Token> tokens) {
        this->type = type;
        this->children = children;
        this->tokens = tokens;
    }

    inline void addChild(Node node) {
        this->children.push_back(node);
    }

    inline void addToken(Token token) {
        this->tokens.push_back(token);
    }

    inline Node childAt(int index) {
        return this->children[index];
    }

    inline Token tokenAt(int index) {
        return this->tokens[index];
    }

    inline void print() {
        this->print("|");
    }

    void print(std::string level) {
        std::cout << "\033[30m" << level << "\033[m" << (int)type << std::endl;

        for(Token token : this->tokens)
            std::cout << "\033[30m" << level << "||" << "\033[m" << ((token.string == "|") ? "[pipe]" : token.string) << std::endl;

        for(Node node : this->children)
            node.print(level + "||");
    }
};
