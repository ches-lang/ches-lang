#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "command.cpp"
#include "compiler.cpp"
#include "console.cpp"
#include "lexer.cpp"

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

#define ERROR(msg)          do { err = true; error(std::string(msg) + " (line: " + std::to_string(getline(index)) + ")"); } while(0);
#define ERROR_EXT(msg)      do { err = true; error(std::string(msg) + " (line: " + std::to_string(getline(index)) + ")", true); } while(0);
#define TYPE(i)             (tokens[i].first)
#define TOKEN(i)            (tokens[i].second)
#define MATCH(i, str)       (std::regex_match(tokens[i].second, std::regex(str)))
#define MATCHEX(i, t, str)  (tokens[i].first == t && std::regex_match(tokens[i].second, std::regex(str)))
#define MATCHTP(i, t)       (tokens[i].first == t)



extern std::string source;
extern std::vector<std::pair<char, std::string>> tokens;
std::vector<std::string> pivot;



class Parser {
public:

    Parser() {

    }

    void parse() {
        std::string res;
        do {
            res = scan();
            if(res != " ") pivot.push_back(res);
            std::cout << res << std::endl;
        } while(res != "\0");
    }

private:

    bool err = false;
    int index = 0;

    std::string scan() {
        std::string res = "";
        int length = 0;
        if(tokens[index].first == EOF) return "\0";
        for(int i = index; i < tokens.size(); i++)
            if(tokens[i].second != "\n") length++;
            else break;
        res = tokens[index].second;

        /* def func */
        if(MATCHEX(index, KEYWORD, "private|public")) {
            // アクセス修飾子 型名 関数名 ( 引数 )
            // アクセス修飾子 型名 変数名
            // アクセス修飾子 型名 変数名 = 値
            if(MATCHEX(index + 1, KEYWORD, "obj|void") || MATCHTP(index, IDENTIFIER))
                res += " yay";
            else ERROR("expected return type");
        }

        index += length + 1;
        return res;
    }

    int getline(int index) {
        int res = 0;
        for(int i = 0; i < index; i++)
            if(source[i] == '\n') res++;
        return res;
    }
};
