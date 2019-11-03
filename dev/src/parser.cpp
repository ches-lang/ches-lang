#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "console.cpp"
#include "syntax.cpp"



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

#define ROOT        33
#define DEFFUNC     34
#define CALLFUNC    35
#define ARGS        36



class Parser {

public:

    std::vector<std::pair<char, std::string>> tokens;
    Node tree = Node(ROOT);

    Parser() {}

    Parser(std::vector<std::pair<char, std::string>> tk) {
        tokens = tk;
    }

    // 構文を簡略化
    Node run() {
        bool b;
        do {
            b = scan();
        } while(b);
        tree.out("");
        return tree;
    }

private:

    int index = 0;
    std::vector<std::pair<char, std::string>> ln;

    bool scan() {
        if(tokens[index].first == EOF) return false;

        while(true) {
            if(tokens[index].first == EOF) {
                break;
            } else if(tokens[index].first == NEWLINE) {
                index++;
                break;
            } else {
                ln.push_back(tokens[index]);
                index++;
            }
        }

        try {
            int len = ln.size();

            if(typeMatch(0, IDENTIFIER) && typeMatch(1, LPAREN) && typeMatch(len - 1, RPAREN)) {
                Node fnc(DEFFUNC);
                fnc.add(Child(at(0)));
                Node args(ARGS);
                for(int i = 2; i < len - 1; i++)
                    args.add(Child(at(i)));
                fnc.add(args);
                tree.add(fnc);
            }

            else if(typeMatch(0, INDENT) && typeMatch(1, IDENTIFIER) && typeMatch(2, LPAREN) && typeMatch(len - 1, RPAREN)) {
                Node callfnc(CALLFUNC);
                callfnc.add(Child(at(1)));
                Node args(ARGS);
                for(int i = 3; i < len - 1; i++)
                    args.add(Child(at(i)));
                callfnc.add(args);
                tree.add(callfnc);
            }
        } catch(std::out_of_range ignored) {}

        ln.clear();
        return true;

        /*
        bool res = true;
        //if(tokens[index].first == EOF) {std::cout << "eof: " << index << std::endl;return false;}

        while(true) {
            if(tokens[index].first == EOF) {
                //index++;
                res = false;
                break;
            } else if(tokens[index].first == NEWLINE) {//std::cout << "newline" << std::endl;
                index++;
                break;
            } else {
                //std::cout << index << " " << ((tokens[index].second == "  ") ? "indent" : tokens[index].second) << std::endl;
                ln.push_back(tokens[index]);
                index++;
            }
        }

        for(std::pair<char, std::string> p : ln) {
            std::cout << ((p.second == "  ") ? "indent" : p.second) << " ";
        } std::cout << "index: " << index << std::endl;

        try {
            int len = ln.size();

            if(typeMatch(0, IDENTIFIER) && typeMatch(1, LPAREN) && typeMatch(len - 1, RPAREN)) {
                Node fnc(DEFFUNC);
                fnc.add(Child(at(0)));
                Node args(ARGS);
                for(int i = 2; i < len - 1; i++)
                    args.add(Child(at(i)));
                fnc.add(args);
                tree.add(fnc);
                return res;
            }

            if(typeMatch(0, INDENT) && typeMatch(1, IDENTIFIER) && typeMatch(2, LPAREN) && typeMatch(len - 1, RPAREN)) {
                Node callfnc(CALLFUNC);
                callfnc.add(Child(at(1)));
                Node args(ARGS);
                callfnc.add(args);
                //tree.add(callfnc);
                return res;
            }



            // ↓のソースコードを...

            main(str[] args)
              println("Hello, world")

            // ↓に変換！

            ROOT
              DEFFUNC
                main
                str
                [
                ]
                args
              CALLFUNC
                println
                Hello, World

            

        } catch(std::out_of_range ignored) {}

        return res;*/
    }

    inline std::pair<char, std::string> at(int i) {
        return ln.at(i);
    }

    bool match(int i, std::pair<char, std::string> p) {
        return (ln.at(i) == p);
    }

    inline char typeAt(int i) {
        return ln.at(i).first;
    }

    inline bool typeMatch(int i, char c) {
        return (typeAt(i) == c);
    }

    inline std::string tokenAt(int i) {
        return ln.at(i).second;
    }

    inline bool tokenMatch(int i, std::string s) {
        return (tokenAt(i) == s);
    }
};
