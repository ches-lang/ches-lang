#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "command.cpp"
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



class Lexer {

public:

    std::string source;

    Lexer() {}

    Lexer(std::string src) {
        source = src;
    }

    std::vector<std::pair<char, std::string>> run() {
        std::vector<std::pair<char, std::string>> res;
        std::pair<char, std::string> p;

        do {
            p = scan();
            res.push_back(p);
            //std::cout << (int)p.first << "\t" << ((p.second == "\n") ? "\\n" : p.second) << "|" << std::endl;
        } while(p.first != ENDOFFILE);
        return res;
    }

private:

    bool err = false;
    int index = -1;

    std::pair<char, std::string> scan() {
        index++;
        char ch = source[index];

        if(index >= source.size())
            return std::make_pair(ENDOFFILE, "");

        else if(ch == '!')
            return std::make_pair(EXCLAMATION, std::string{ch});

        else if(ch == '?')
            return std::make_pair(QUESTION, std::string{ch});

        else if(ch == '~')
            return std::make_pair(TILDE, std::string{ch});

        else if(ch == '+')
            return std::make_pair(PLUS, std::string{ch});

        else if(ch == '-')
            return std::make_pair(HYPHEN, std::string{ch});

        else if(ch == '*')
            return std::make_pair(ASTERISK, std::string{ch});

        else if(ch == '/') {
            if(source[index + 1] == '*') {
                std::string res;
                for(index += 2; index < source.length(); index++) {
                    if(source[index] == '*' && source[index + 1] == '/') {
                        index++;
                        return std::make_pair(COMMENTOUT, res);
                    } else res += std::string{source[index]};
                }
                error("cerr1562", "expected EOF", { "line", std::to_string(getline(index)), "expected", "'*/'" }, true);
            }
            else
                return std::make_pair(SLASH, std::string{ch});
        }

        else if(ch == '%')
            return std::make_pair(PERCENTAGE, std::string{ch});

        else if(ch == '^')
            return std::make_pair(TILDE, std::string{ch});

        else if(ch == '=')
            return std::make_pair(EQUAL, std::string{ch});

        else if(ch == '|')
            return std::make_pair(PIPE, std::string{ch});

        else if(ch == '&')
            return std::make_pair(AMPERSAND, std::string{ch});

        else if(ch == '.')
            return std::make_pair(PERIOD, std::string{ch});

        else if(ch == ',')
            return std::make_pair(COMMA, std::string{ch});

        else if(ch == ':')
            return std::make_pair(COLON, std::string{ch});

        else if(ch == ';')
            return std::make_pair(SEMICOLON, std::string{ch});

        else if(ch == '(')
            return std::make_pair(LPAREN, std::string{ch});

        else if(ch == ')')
            return std::make_pair(RPAREN, std::string{ch});

        else if(ch == '[')
            return std::make_pair(LBRACK, std::string{ch});

        else if(ch == ']')
            return std::make_pair(RBLACK, std::string{ch});

        else if(ch == '<')
            return std::make_pair(LANGBLACK, std::string{ch});

        else if(ch == '>')
            return std::make_pair(RANGBLACK, std::string{ch});

        else if(ch == '{')
            return std::make_pair(LBRACE, std::string{ch});

        else if(ch == '}')
            return std::make_pair(RBRACE, std::string{ch});

        else if(ch == ' ') {
            for(int i = index - 1; i >= 0; i--)
                if(source[i] == ' ') continue;
                else if(source[i] == '\n') break;
                else return scan();

            if(source[index + 1] == ' ') {
                index += 1;
                return std::make_pair(INDENT, "  ");
            } else {
                if(source[index + 1] == ' ') index++;
                err = true;
                error("cerr7903", "invalid indent space", { "line", std::to_string(getline(index)) }, false);
            }
        }

        else if(ch == '\n')
            if(source[index - 1] == '\n') return scan();
            else return std::make_pair(NEWLINE, std::string{ch});

        else if(std::regex_match(std::string{ch}, std::regex("[0-9]"))) {
            std::string num;
            std::string str;
            for(; index < source.length(); index++) {
                str = std::string{source[index]};
                if(std::regex_match(str, std::regex("[0-9]")))
                    num += str;
                else { index--; break; }
            }
            return std::make_pair(NUMBER, num);
        }

        else if(ch == '\'') {
            std::string chr = "";
            if(source[index + 2] == '\'') {
                chr = std::string{source[index + 1]};
                index += 2;
            } else if(source[index + 1] == '\\' && source[index + 3] == '\'') {
                chr = "\\" + std::string{source[index + 2]};
                index += 3;
            } else if(source[index + 1] == '\'') {
                error("cerr4139", "too short character length", { "line", std::to_string(getline(index)), "expected", "a character" }, false);
                index += 1;
            } else {
                bool contain = false;
                int i;
                for(index += 1; index < source.length(); index++)
                    if(source[index] == '\'') { contain = true; break; }
                if(contain) {
                    err = true;
                    error("cerr2471", "too long character length", { "line", std::to_string(getline(index)) }, false);
                }
                else error("cerr1562", "expected EOF", { "line", std::to_string(getline(index)), "expected", "'''" }, true);
                return scan();
            }
            return std::make_pair(CHARACTER, std::string{chr});
        }

        else if(ch == '\"') {
            std::string res;
            for(index += 1; index < source.length(); index++) {
                if(source[index] != '\"') res += source[index];
                else return std::make_pair(STRING, res);
            }
            error("cerr1562", "expected EOF", { "line", std::to_string(getline(index)), "expected", "'\"'" }, true);
        }

        else {
            std::string res;
            while(index < source.length()) {
                // 正規表現では↑で判定済みの文字のみを使用してください。(無限ループ防止)
                if(!std::regex_match(std::string{source[index]}, std::regex("[!?~+\\-*/%^=&|.,:;()\\[\\]<>{}'\" \n]"))) {
                    res += source[index];
                    index++;
                }
                else { index--; break; }
            }
            if(std::regex_match(res, std::regex("bol|break|byt|default|case|catch|chr|class|continue|dbl|elif|else|false|flt|for|import|int|lon|new|null|obj|package|private|public|return|sht|str|switch|true|ubyt|usht|uint|ulon|void")))
                return std::make_pair(KEYWORD, res);
            else
                return std::make_pair(IDENTIFIER, res);
        }

        return std::make_pair(UNKNOWN, std::string{ch});
    }

    int getline(int index) {
        int res = 0;
        for(int i = 0; i < index; i++)
            if(source[i] == '\n') res++;
        return res;
    }
};
