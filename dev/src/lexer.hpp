#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>
#include "command.cpp"
#include "console.cpp"
#include "syntax.cpp"



/* ! ? ~ + - * / % ^ = | & . , : ; ( ) [ ] < > { } */
/*#define ENDOFFILE   0
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
#define RBRACE      34*/



//#ifndef INCLUDED_OPTIONS
//#define INCLUDED_OPTIONS

struct Options {

    std::unordered_map<std::string, std::string> map;

    Options();

    Options(std::unordered_map<std::string, std::string> opt);

    bool exists(std::string key);

    std::string get(std::string key);

    void set(std::string key, std::string value);

    int size();
};

//#endif



class Lexer {

public:

    std::string source;
    Options options;

    Lexer();

    Lexer(std::string src, Options opt);

    std::vector<Token> getTokens();

private:

    int index = -1;

    Token scan();
};
