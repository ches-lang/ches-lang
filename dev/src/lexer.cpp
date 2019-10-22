#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>
#include "command.cpp"
#include "compiler.cpp"
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



std::string source;
std::vector<std::pair<char, std::string>> tokens;



class Lexer {

public:

    Lexer() {

    }

    void analyze() {
        std::pair<char, std::string> res;
        do {
            res = scan();
            tokens.push_back(res);

            if(res.second == "\n") res.second = "\\n";
            std::cout << "type " << (int)res.first << "\t" << res.second << std::endl;

        } while(res.first != ENDOFFILE);
        if(err) exit(-1);
    }

private:

    bool err = false;
    int index = -1;

    std::pair<char, std::string> scan() {
        index++;
        char ch = source[index];

        if(index >= source.length())
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

        else if(ch == '/')
            if(source[index + 1] == '*') {
                std::string res;
                for(index += 2; index < source.length(); index++) {
                    if(source[index] == '*' && source[index + 1] == '/') {
                        index++;
                        return std::make_pair(COMMENTOUT, res);
                    } else res += std::string{source[index]};
                }
                error("expected eof with '*/' (line: " + std::to_string(getline(index)) + ")", true);
            }
            else
                return std::make_pair(SLASH, std::string{ch});

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

            if(source[index + 1] == ' ' && source[index + 2] == ' ' && source[index + 3] == ' ') {
                index += 3;
                return std::make_pair(INDENT, "    ");
            } else {
                int add = 0;
                if(source[index + 1] == ' ') add++;
                if(source[index + 2] == ' ') add++;
                if(source[index + 3] == ' ') add++;
                index += add;
                err = true;
                error("invalid indent space (line: " + std::to_string(getline(index)) + ")");
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
            } else {
                bool contain = false;
                int i;
                for(index += 1; index < source.length(); index++)
                    if(source[index] == '\'') { contain = true; break; }
                if(contain) {
                    err = true;
                    error("too long character (line: " + std::to_string(getline(index)) + ")");
                }
                else error("expected eof with ''' (line: " + std::to_string(getline(index)) + ")", true);
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
            error("expected eof with '\"' (line: " + std::to_string(getline(index)) + ")", true);
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

        return std::make_pair(UNKNOWN, "");
    }

    int getline(int index) {
        int res = 0;
        for(int i = 0; i < index; i++)
            if(source[i] == '\n') res++;
        return res;
    }
};
