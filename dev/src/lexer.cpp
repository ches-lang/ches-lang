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
#define KEYWORD     3
#define IDENTIFIER  4
#define NUMBER      5
#define CHARACTER   6
#define STRING      7
#define EXCLAMATION 8
#define QUESTION    9
#define TILDE       10
#define PLUS        11
#define HYPHEN      12
#define ASTERISK    13
#define SLASH       14
#define PERCENTAGE  15
#define CARET       16
#define EQUAL       17
#define PIPE        18
#define AMPERSAND   19
#define PERIOD      20
#define COMMA       21
#define COLON       22
#define SEMICOLON   23
#define LPAREN      24
#define RPAREN      25
#define LBRACK      26
#define RBLACK      27
#define LANGBLACK   28
#define RANGBLACK   29
#define LBRACE      30
#define RBRACE      31



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
    }

private:

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

        else if(ch == '/') // コメントアウト
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
            } else error("invalid indent space");
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
                for(i = index + 1; i < source.length(); i++)
                    if(source[i] == '\'') { contain = true; break; }
                index = i;
                if(contain) error("too long character");
                else error("expected eof with '''");
                return scan();
            }
            return std::make_pair(CHARACTER, std::string{chr});
        }

        else if(ch == '\"') {
            std::string res;
            for(int i = index + 1; i < source.length(); i++)
                if(source[i] != '\"') res += source[i];
                else {
                    index += res.length() + 1;
                    return std::make_pair(STRING, res);
                }
            error("expected eof with '\"'");
        }

        else {
            std::string res;
            while(index < source.length()) {
                // 正規表現では↑で判定済みの文字のみを使用してください。(無限ループ防止)
                if(!std::regex_match(std::string{source[index]}, std::regex("[!?~+\\-*/%^=&|.,:;()\\[\\]<>{}'\" \n]"))) {
                    /* ! ? ~ + - * / % ^ = | & . , : ; ( ) [ ] < > { } */
                    res += source[index];
                    index++;
                }
                else { index--; break; }
            }
            return std::make_pair(IDENTIFIER, res);
        }
        return std::make_pair(UNKNOWN, "");
    }
};
