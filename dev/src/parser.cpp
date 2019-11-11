#pragma once

#include <iostream>
#include <regex>
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

#define at(i) (tk.at(i))
#define match(i, t) (t.compare(at(i)))
#define typeAt(i) (at(i).type)
#define typeMatch(i, c) (typeAt(i) == c)
#define stringAt(i) (at(i).string)
#define stringMatch(i, s) (stringAt(i) == s)
#define stringMatchReg(i, s) (std::regex_match(stringAt(i), std::regex(s)))



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

            if(indent == 0 && typeMatch(0, IDENTIFIER) && typeMatch(1, LPAREN) && typeMatch(len - 1, RPAREN)) {
                Node node("DEFFUNC");
                node.addToken(Token(IDENTIFIER, stringAt(0)));
                if(len >= 4) {
                    Node args("ARGS");
                    std::vector<Token> ag;
                    int nest = 0;
                    for(int i = 2; i < len - 1; i++) {
                        if(stringAt(i) == "," && nest == 0) {
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                            ag.clear();
                        } else if(i == len - 2) {
                            ag.push_back(at(i));
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                        } else {
                            if(stringMatch(i, "[(\\[{]"))
                                nest++;
                            else if(stringMatch(i, "[)\\}]"))
                                nest--;
                            ag.push_back(at(i));
                        }
                    }
                    node.addNode(args);
                }

                return node;
            }

            if(indent >= 1 && len >= 3 && typeMatch(0, IDENTIFIER) && typeMatch(1, LPAREN) && typeMatch(len - 1, RPAREN)) {
                Node node("CALLFUNC");
                node.addToken(Token(IDENTIFIER, stringAt(0)));
                if(len >= 4) {
                    Node args("ARGS");
                    std::vector<Token> ag;
                    int nest = 0;
                    for(int i = 2; i < len - 1; i++) {
                        if(stringAt(i) == "," && nest == 0) {
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                            ag.clear();
                        } else if(i == len - 2) {
                            ag.push_back(at(i));
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                        } else {
                            if(stringMatchReg(i, "[(\\[{]"))
                                nest++;
                            else if(stringMatchReg(i, "[)\\]}]"))
                                nest--;
                            ag.push_back(at(i));
                        }
                    }
                    node.addNode(args);
                }
                return node;
            }

            if(len == 2 && (typeMatch(0, IDENTIFIER) || typeMatch(0, KEYWORD)) && typeMatch(1, IDENTIFIER)) {
                Node node("DEFVAR");
                node.addToken(at(0));
                node.addToken(at(1));
                return node;
            }

            if(len == 4 &&  (typeMatch(0, IDENTIFIER) || typeMatch(0, KEYWORD)) && typeMatch(1, LBRACK) && typeMatch(2, RBRACK) && typeMatch(3, IDENTIFIER)) {
                Node node("DEFVAR");
                node.addToken(at(0));
                node.addToken(at(1));
                node.addToken(at(2));
                node.addToken(at(3));
                return node;
            }

            if(len >= 4 && (typeMatch(0, IDENTIFIER) || typeMatch(0, KEYWORD)) && typeMatch(1, IDENTIFIER) && typeMatch(2, EQUAL)) {
                Node node("INITVAR");
                node.addToken(at(0));
                node.addToken(at(1));
                if(len == 4) {
                    node.addToken(at(3));
                } else {
                    std::vector<Token> rs;
                    for(int i = 3; i < len; i++)
                        rs.push_back(at(i));
                    node.addNode(getNode(rs));
                }
                return node;
            }

            if(len >= 4 && match(0, Token(KEYWORD, "for")) && typeMatch(1, LPAREN) && typeMatch(len - 1, RPAREN)) {
                Node node("NODE");
                int sccount = 0;

                for(Token t : tk)
                    if(t.type == SEMICOLON) sccount++;

                if(sccount == 0) {
                    if(len == 4) {
                        node.addNode(Node("COUNT", {}, { at(2) }));
                    } else {
                        std::vector<Token> inParen;
                        for(int i = 2; i < len - 1; i++)
                            inParen.push_back(at(i));
                        node.addNode(getNode(inParen));
                    }
                } else if(sccount == 2) {
                    std::vector<Token> item;
                    for(int i = 2; i < len; i++) {
                        if(typeMatch(i, SEMICOLON)) {
                            node.addNode(getNode(item));
                            item.clear();
                        } else {
                            item.push_back(at(i));
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
                if(typeMatch(i, LPAREN)) {
                    if(countlpc) lpcount++;
                } else {
                    if(stringMatchReg(i, "[~+\\-*/%^=&|<>]"))
                        ismatch = true;
                    countlpc = false;
                }
            }
            if(len >= 3 && ismatch) {
                Node node("EXPRESS");
                for(int i = lpcount; i < len; i++) {
                    
                }
                return node;
            }

        } catch(std::out_of_range ignored) {
            std::cout << "EXCEPTION" << std::endl;
        }

        return Node("UNKNOWN");
    }
};
