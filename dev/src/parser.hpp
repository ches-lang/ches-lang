#pragma once

#include <deque>
#include <iostream>
#include <regex>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include "console.cpp"
#include "lexer.cpp"
#include "syntax.cpp"



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

#define A(i) (tk.at(i >= 0 ? i : len - 1))
#define M(i, t, s) (Token(t, s).compare(A(i)))
#define IA(i) (A(i).index)
#define TA(i) (A(i).type)
#define TM(i, c) (TA(i) == c)
#define SA(i) (A(i).string)
#define SM(i, s) (SA(i) == s)
#define SMR(i, s) (std::regex_match(SA(i), std::regex(s)))



class Parser {

private:

    std::string sourcePath;
    std::string source;
    std::vector<Token> tokens;
    Options options;
    Node tree = Node(N_ROOT);

    int index = 0;
    std::vector<Token> ln;
    int indent = 0;

public:

    Parser();

    Parser(std::string srcpath, std::string src, std::vector<Token> tk, Options opt);

    Node parse();

private:

    bool scan();

    Node getNode(std::vector<Token> tk);

    // 優先度高い             優先度低い
    // true → ope1 < ope2   false → ope1 >= ope2
    bool compareOpe(std::string ope1, std::string ope2);

    std::vector<Token> copy(int begin, int length, std::vector<Token> src);
};
