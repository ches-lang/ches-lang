#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "console.cpp"
#include "parser.cpp"

/*
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
#define N_FREEVAR   0x05    // 不明
#define N_DEFFUNC   0x06
#define N_CALLFUNC  0x07
#define N_ARGS      0x08
#define N_IF        0x09
#define N_ELSE      0x10
#define N_ELSEIF    0x11
#define N_LOOP      0x12
#define N_LOOP_COND 0x13
#define N_LOOP_INIT 0x14
#define N_LOOP_CHNG 0x15
#define N_COUNT     0x16
#define N_LOGIC     0x17
#define N_COMP      0x18
#define N_EXPRESS   0x19    // 不明
#define N_ITEM      0x20    // 不要？
#define N_OPE       0x21
#define N_EQUAL     0x22
#define N_LESS      0x23
#define N_GREATER   0x24
#define N_LESSEQL   0x25
#define N_GRTREQL   0x26

#define SI_BCSTART  0x00    // 不明
#define SI_LNDIV    0x01
#define SI_TKDIV    0x02
#define SI_LLPREF   0x03    // 不明

#define I_UNKNOWN   0x04
#define I_GROUP     0x05
#define I_LSPUSH    0x06
#define I_LLPUSH    0x07
#define I_LAND      0x08
#define I_LOR       0x09
#define I_NADD      0x12
#define I_NSUB      0x13
#define I_NMUL      0x14
#define I_NDIV      0x15
#define I_JUMP      0x16
#define I_IFJUMP    0x17
*/



/* ! ? ~ + - * / % ^ = | & . , : ; ( ) [ ] < > { } */
enum TokenType : unsigned char {
    TK_Unknown,
    TK_EndOfFile,
    TK_Indent,
    TK_NewLine,
    TK_CommentOut,
    TK_Keyword,
    TK_Identifier,
    TK_Number,
    TK_Character,
    TK_String,
    TK_Exclamation,
    TK_Question,
    TK_Tilde,
    TK_Plus,
    TK_Hyphen,
    TK_Asterisk,
    TK_Slash,
    TK_Percentage,
    TK_Caret,
    TK_Equal,
    TK_Pipe,
    TK_Ampersand,
    TK_Period,
    TK_Comma,
    TK_Colon,
    TK_Semicolon,
    TK_LeftParen,
    TK_RightParen,
    TK_LeftBracket,
    TK_RightBracket,
    TK_LeftAngleBracket,
    TK_RightAngleBracket,
    TK_LeftBrace,
    TK_RightBrace,
};



enum NodeType : unsigned char {
    ND_Unknown,
    ND_Root,
    ND_Token,
    ND_DefVariable,
    ND_InitVariable,
    ND_DefFunction,
    ND_CallFunction,
    ND_Args,
    ND_If,
    ND_Else,
    ND_ElseIf,
    ND_Loop,
    ND_Loop_Condition,
    ND_Loop_Init,
    ND_Loop_Change,
    ND_Count,
    ND_Logic,
    ND_Compare,
    ND_Expression,
    ND_Operator,
    ND_Equal,
    ND_Less,
    ND_Greater,
    ND_LessEqual,
    ND_GreaterEqual,
};



enum InstType : unsigned char {
    IT_Unknown,
    IT_LineDivide,
    IT_TokenDivide,
    IT_Label,
    IT_LocalStackPush,
    IT_LocalListPush,
    IT_Logic_And,
    IT_Logic_Or,
    IT_Calc_Add,
    IT_Calc_Subtract,
    IT_Calc_Multiplicate,
    IT_Calc_Divide,
    IT_Jump,
    IT_IFJump,
};



struct Token {

    int index = 0;
    unsigned char type = TK_Unknown;
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

    bool isParen();

    bool isCloseParen();

    bool isOpenParen();

    bool match(std::vector<unsigned char> matches);

    bool match(std::string regexp);
};



struct Node {

    unsigned char type = ND_Unknown;
    std::vector<Node> children;
    std::vector<Token> tokens;
    std::string prefix = "||";

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
