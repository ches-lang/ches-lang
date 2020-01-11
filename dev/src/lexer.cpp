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



//#ifndef INCLUDED_OPTIONS
//#define INCLUDED_OPTIONS

struct Options {

    std::unordered_map<std::string, std::string> map;

    Options() {}

    Options(std::unordered_map<std::string, std::string> opt) {
        map = opt;
    }

    bool exists(std::string key) {
        return (map.find(key) != map.end());
    }

    std::string get(std::string key) {
        return map[key];
    }

    void set(std::string key, std::string value) {
        map[key] = value;
    }

    int size() {
        return map.size();
    }
};

//#endif



class Lexer {

public:

    std::string source;
    Options options;

    Lexer() {}

    Lexer(std::string src, Options opt) {
        source = src;
        options = opt;
    }

    std::vector<Token> getTokens() {
        std::vector<Token> tokens;
        Token tk;

        do {
            tk = scan();
            tokens.push_back(tk);
            //std::cout << (int)tk.type << "\t" << ((tk.string == "\n") ? "\\n" : tk.string) << std::endl;//
        } while(tk.type != ENDOFFILE);

        if(Console::errored) {
            exit(-1);
        } else if(Console::warned) {
            if(!options.exists("-miss")) {
                exit(-1);
            }
        }

        return tokens;
    }

private:

    int index = -1;

    Token scan() {
        index++;
        char ch = source[index];

        if(index >= source.size())
            return Token(ENDOFFILE, "", index);

        else if(ch == '!')
            return Token(EXCLAMATION, std::string{ch}, index);

        else if(ch == '?')
            return Token(QUESTION, std::string{ch}, index);

        else if(ch == '~')
            return Token(TILDE, std::string{ch}, index);

        else if(ch == '+')
            return Token(PLUS, std::string{ch}, index);

        else if(ch == '-')
            return Token(HYPHEN, std::string{ch}, index);

        else if(ch == '*')
            return Token(ASTERISK, std::string{ch}, index);

        else if(ch == '/') {
            if(source[index + 1] == '*') {
                std::string res;
                int start = index;
                for(index += 2; index < source.length(); index++) {
                    if(source[index] == '*' && source[index + 1] == '/') {
                        index++;
                        return Token(COMMENTOUT, res, start);
                    } else res += std::string{source[index]};
                }
                Console::error("cerr1562", "expected EOF", { { "at", Token::getPositionText(options.get("-i"), source, index) }, { "expected", "'*/'" } }, true);
            } else if(source[index + 1] == '/') {
                std::string res;
                int start = index;
                for(index += 2; index < source.length(); index++) {
                    if(source[index] == '\n') {
                        index--;
                        break;
                    }
                    res += std::string{source[index]};
                }
                return Token(COMMENTOUT, res, start);
            } else {
                return Token(SLASH, std::string{ch}, index);
            }
        }

        else if(ch == '%')
            return Token(PERCENTAGE, std::string{ch}, index);

        else if(ch == '^')
            return Token(TILDE, std::string{ch}, index);

        else if(ch == '=')
            return Token(EQUAL, std::string{ch}, index);

        else if(ch == '|')
            return Token(PIPE, std::string{ch}, index);

        else if(ch == '&')
            return Token(AMPERSAND, std::string{ch}, index);

        else if(ch == '.')
            return Token(PERIOD, std::string{ch}, index);

        else if(ch == ',')
            return Token(COMMA, std::string{ch}, index);

        else if(ch == ':')
            return Token(COLON, std::string{ch}, index);

        else if(ch == ';')
            return Token(SEMICOLON, std::string{ch}, index);

        else if(ch == '(')
            return Token(LPAREN, std::string{ch}, index);

        else if(ch == ')')
            return Token(RPAREN, std::string{ch}, index);

        else if(ch == '[')
            return Token(LBRACK, std::string{ch}, index);

        else if(ch == ']')
            return Token(RBRACK, std::string{ch}, index);

        else if(ch == '<')
            return Token(LANGBRACK, std::string{ch}, index);

        else if(ch == '>')
            return Token(RANGBRACK, std::string{ch}, index);

        else if(ch == '{')
            return Token(LBRACE, std::string{ch}, index);

        else if(ch == '}')
            return Token(RBRACE, std::string{ch}, index);

        else if(ch == ' ') {
            for(int i = index - 1; i >= 0; i--)
                if(source[i] == ' ') continue;
                else if(source[i] == '\n') break;
                else return scan();

            if(source[index + 1] == ' ') {
                index += 1;
                return Token(INDENT, "  ", index - 1);
            } else {
                if(source[index + 1] == ' ') index++;
                Console::error("cerr7903", "invalid indent space", { { "at", Token::getPositionText(options.get("-i"), source, index) } }, false);
            }
        }

        else if(ch == '\n')
            if(source[index - 1] == '\n') return scan();
            else return Token(NEWLINE, std::string{ch}, index);

        else if(std::regex_match(std::string{ch}, std::regex("[0-9]"))) {
            std::string num;
            std::string str;
            int start = index;
            for(; index < source.length(); index++) {
                str = std::string{source[index]};
                if(std::regex_match(str, std::regex("[0-9]")))
                    num += str;
                else { index--; break; }
            }
            return Token(NUMBER, num, start);
        }

        else if(ch == '\'') {
            std::string chr = "";
            int start = index;
            if(source[index + 2] == '\'') {
                chr = std::string{source[index + 1]};
                index += 2;
            } else if(source[index + 1] == '\\' && source[index + 3] == '\'') {
                chr = "\\" + std::string{source[index + 2]};
                index += 3;
            } else if(source[index + 1] == '\'') {
                Console::error("cerr4139", "too short character length", { { "at", Token::getPositionText(options.get("-i"), source, index) }, { "expected", "a character" } }, false);
                index += 1;
            } else {
                bool contain = false;
                int i;
                for(index += 1; index < source.length(); index++)
                    if(source[index] == '\'') { contain = true; break; }
                if(contain) {
                    Console::error("cerr2471", "too long character length", { { "at", Token::getPositionText(options.get("-i"), source, index) } }, false);
                } else {
                    Console::error("cerr1562", "expected EOF", { { "at", Token::getPositionText(options.get("-i"), source, index) }, { "expected", "'''" } }, true);
                }
                return scan();
            }
            return Token(CHARACTER, std::string{chr}, start);
        }

        else if(ch == '\"') {
            std::string res;
            int start = index;
            for(index += 1; index < source.length(); index++) {
                if(source[index] != '\"') res += source[index];
                else return Token(STRING, res, start);
            }
            Console::error("cerr1562", "expected EOF", { { "at", Token::getPositionText(options.get("-i"), source, index) }, { "expected", "'\"'" } }, true);
        }

        else {
            std::string res;
            int start = index;
            while(index < source.length()) {
                // 正規表現では↑で判定済みの文字のみを使用してください。(無限ループ防止)
                if(!std::regex_match(std::string{source[index]}, std::regex("[!?~+\\-*/%^=&|.,:;()\\[\\]<>{}'\" \n]"))) {
                    res += source[index];
                    index++;
                }
                else { index--; break; }
            }
            if(std::regex_match(res, std::regex("bol|break|byt|default|case|catch|chr|class|continue|dbl|elif|else|false|flt|for|import|int|lon|new|null|obj|package|private|public|return|sht|str|switch|true|ubyt|usht|uint|ulon|void"))) {
                return Token(KEYWORD, res, start);
            } else {
                return Token(IDENTIFIER, res, start);
            }
        }

        return Token(UNKNOWN, std::string{ch});
    }
};
