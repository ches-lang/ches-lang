#pragma once

#include <iostream>
#include <regex>
#include <stack>
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
#define RBRACK      28
#define LANGBLACK   29
#define RANGBLACK   30
#define LBRACE      31
#define RBRACE      32

#define A(i) (tk.at(i))
#define M(i, t) (t.compare(A(i)))
#define TA(i) (A(i).type)
#define TM(i, c) (TA(i) == c)
#define SA(i) (A(i).string)
#define SM(i, s) (SA(i) == s)
#define SMR(i, s) (std::regex_match(SA(i), std::regex(s)))



class Parser {

public:

    std::vector<Token> tokens;
    Node tree = Node("ROOT");

    Parser() {}

    Parser(std::vector<Token> tk) {
        tokens = tk;
    }

    Node run() {
        bool b;
        do {
            b = scan();
        } while(b);
        //std::cout << tree.getNode(0).getNode(0).tokens[0].string << std::endl;
        tree.out("");
        return tree;
    }



private:

    int index = 0;
    std::vector<Token> ln;
    int indent = 0;

    bool scan() {
        if(tokens[index].type == EOF) return false;

        Token tk;
        while(true) {
            tk = tokens[index];
            if(tk.type == EOF) {
                break;
            } else if(tk.type == NEWLINE) {
                index++;
                break;
            } else if(tk.type == INDENT) {
                indent++;
                index++;
            } else if(tk.type == COMMENTOUT) {
                index++;
            } else {
                ln.push_back(tokens[index]);
                index++;
            }
        }

        Node n = getNode(ln);
        if(!n.isEmpty()) tree.addNode(getNode(ln));

        indent = 0;
        ln.clear();
        return true;
    }

    Node getNode(std::vector<Token> tk) {
        int len = tk.size();
        if(len == 0 || len == 1) return Node();

        for(Token t : tk) {
            std::cout << t.string << " ";
        } std::cout << std::endl;

        try {

            if(indent == 0 && TM(0, IDENTIFIER) && TM(1, LPAREN) && TM(len - 1, RPAREN)) {
                Node node("DEFFUNC");
                node.addToken(Token(IDENTIFIER, SA(0)));
                if(len >= 4) {
                    Node args("ARGS");
                    std::vector<Token> ag;
                    int nest = 0;
                    for(int i = 2; i < len - 1; i++) {
                        if(SA(i) == "," && nest == 0) {
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                            ag.clear();
                        } else if(i == len - 2) {
                            ag.push_back(A(i));
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                        } else {
                            if(SM(i, "[(\\[{]"))
                                nest++;
                            else if(SM(i, "[)\\}]"))
                                nest--;
                            ag.push_back(A(i));
                        }
                    }
                    node.addNode(args);
                }

                return node;
            }

            if(indent >= 1 && len >= 3 && TM(0, IDENTIFIER) && TM(1, LPAREN) && TM(len - 1, RPAREN)) {
                Node node("CALLFUNC");
                node.addToken(Token(IDENTIFIER, SA(0)));
                if(len >= 4) {
                    Node args("ARGS");
                    std::vector<Token> ag;
                    int nest = 0;
                    for(int i = 2; i < len - 1; i++) {
                        if(SA(i) == "," && nest == 0) {
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                            ag.clear();
                        } else if(i == len - 2) {
                            ag.push_back(A(i));
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                        } else {
                            if(SMR(i, "[(\\[{]"))
                                nest++;
                            else if(SMR(i, "[)\\]}]"))
                                nest--;
                            ag.push_back(A(i));
                        }
                    }
                    node.addNode(args);
                }
                return node;
            }

            if(len == 2 && (TM(0, IDENTIFIER) || TM(0, KEYWORD)) && TM(1, IDENTIFIER)) {
                Node node("DEFVAR");
                node.addToken(A(0));
                node.addToken(A(1));
                return node;
            }

            if(len == 4 &&  (TM(0, IDENTIFIER) || TM(0, KEYWORD)) && TM(1, LBRACK) && TM(2, RBRACK) && TM(3, IDENTIFIER)) {
                Node node("DEFVAR");
                node.addToken(A(0));
                node.addToken(A(1));
                node.addToken(A(2));
                node.addToken(A(3));
                return node;
            }

            if(len >= 4 && (TM(0, IDENTIFIER) || TM(0, KEYWORD)) && TM(1, IDENTIFIER) && TM(2, EQUAL)) {
                Node node("INITVAR");
                node.addToken(A(0));
                node.addToken(A(1));
                if(len == 4) {
                    node.addToken(A(3));
                } else {
                    std::vector<Token> rs;
                    for(int i = 3; i < len; i++)
                        rs.push_back(A(i));
                    node.addNode(getNode(rs));
                }
                return node;
            }

            if(len >= 4 && M(0, Token(KEYWORD, "for")) && TM(1, LPAREN) && TM(len - 1, RPAREN)) {
                Node node("NODE");
                int sccount = 0;

                for(Token t : tk)
                    if(t.type == SEMICOLON) sccount++;

                if(sccount == 0) {
                    if(len == 4) {
                        node.addNode(Node("COUNT", {}, { A(2) }));
                    } else {
                        std::vector<Token> inParen;
                        for(int i = 2; i < len - 1; i++)
                            inParen.push_back(A(i));
                        node.addNode(getNode(inParen));
                    }
                } else if(sccount == 2) {
                    std::vector<Token> item;
                    for(int i = 2; i < len; i++) {
                        if(TM(i, SEMICOLON)) {
                            node.addNode(getNode(item));
                            item.clear();
                        } else {
                            item.push_back(A(i));
                        }
                    }
                }
                return node;
            }

            // 計算式、比較式、論理式
            bool ismatch = false;
            bool countlpc = true;
            int lpcount = 0;
            for(int i = 0; i < len; i++) {
                if(TM(i, LPAREN)) {
                    if(countlpc) lpcount++;
                } else {
                    if(SMR(i, "[~+\\-*/%^=&|<>]"))
                        ismatch = true;
                    countlpc = false;
                }
            }
            if(len >= 3 && ismatch) {
                Node node("EXPRESS");
                std::stack<Token> stack;
                std::vector<Token> item;

                for(int i = lpcount; i < len; i++) {
                    if(std::regex_match(SA(i), std::regex("[+\\-*/]"))) {
                        node.addNode(Node("OPE", {}, { A(i) }));
                    } else {
                        for(int j = i; j <= len; j++) {
                            if(j != len && !std::regex_match(SA(j), std::regex("[+\\-*/]"))) {
                                item.push_back(A(j));
                            } else {
                                if(j == len) i = j + 1;
                                i = i + item.size() - 1;
                                if(item.size() == 1) node.addNode(Node("ITEM", {}, { item.at(0) }));
                                else node.addNode(getNode(item));
                                item.clear();
                                break;
                            }
                        }
                    }
                }
                return node;
            }

        } catch(std::out_of_range ignored) {
            std::cout << "EXCEPTION" << std::endl;
        }

        return Node("UNKNOWN");
    }

    // 優先度高い             優先度低い
    // true → ope1 < ope2   false → ope1 >= ope2
    bool compareOpe(std::string ope1, std::string ope2) {

        if(std::regex_match(ope1, std::regex("[+\\-]"))) {
            return (std::regex_match(ope2, std::regex("[+\\-*/]")));
        }

        if(std::regex_match(ope1, std::regex("[*/]"))) {
            return (std::regex_match(ope2, std::regex("[*/]")));
        }

        return false;
    }
};
