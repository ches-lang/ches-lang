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
    Node tree = Node(N_ROOT);

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
        //tree.out("|");
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

        try {

            /*std::cout << "tk: ";
            for(Token t : tk)
                std::cout << t.string << " ";
            std::cout << std::endl;*/

            if(indent == 0 && TM(0, IDENTIFIER) && TM(1, LPAREN) && TM(len - 1, RPAREN)) {
                Node node(N_DEFFUNC);
                node.addToken(Token(IDENTIFIER, SA(0)));
                if(len >= 4) {
                    Node args(N_ARGS);
                    std::vector<Token> ag;
                    int nest = 0;
                    for(int i = 2; i < len - 1; i++) {
                        if(TM(i, COMMA) && nest == 0) {
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                            ag.clear();
                        } else if(i == len - 2) {
                            ag.push_back(A(i));
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                        } else {
                            if(match(TA(i), { LPAREN, LBRACK, LBRACE }))
                                nest++;
                            else if(match(TA(i), { RPAREN, RBRACK, RBRACE }))
                                nest--;
                            ag.push_back(A(i));
                        }
                    }
                    node.addNode(args);
                }

                return node;
            }

            bool ismatch = true;
            int nest = 0;

            for(int i = 0; i < len; i++) {
                if(match(TA(i), { LPAREN, LBRACK, LBRACE })) {
                    nest++;
                } else if(match(TA(i), { RPAREN, RBRACK, RBRACE })) {
                    nest--;
                    if(nest < 0 || (nest == 0 && i != len - 1)) ismatch = false;
                }
            }

            if(ismatch && nest == 0 && indent >= 1 && len >= 3 && TM(0, IDENTIFIER) && TM(1, LPAREN) && TM(len - 1, RPAREN)) {
                Node node(N_CALLFUNC);
                node.addToken(Token(IDENTIFIER, SA(0)));

                nest = 0;

                if(len >= 4) {
                    Node args(N_ARGS);
                    std::vector<Token> ag;
                    for(int i = 2; i < len - 1; i++) {
                        if(TM(i, COMMA) && nest == 0) {
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                            ag.clear();
                        } else if(i == len - 2) {
                            ag.push_back(A(i));
                            if(ag.size() == 1) args.addToken(ag[0]);
                            else args.addNode(getNode(ag));
                        } else {
                            if(match(TA(i), { LPAREN, LBRACK, LBRACE })) {
                                nest++;
                            } else if(match(TA(i), { RPAREN, RBRACK, LBRACE })) {
                                nest--;
                            }
                            ag.push_back(A(i));
                        }
                    }
                    node.addNode(args);
                }
                return node;
            }

            if(len == 2 && (TM(0, IDENTIFIER) || TM(0, KEYWORD)) && TM(1, IDENTIFIER)) {
                Node node(N_DEFVAR);
                node.addToken(A(0));
                node.addToken(A(1));
                return node;
            }

            if(len == 4 &&  (TM(0, IDENTIFIER) || TM(0, KEYWORD)) && TM(1, LBRACK) && TM(2, RBRACK) && TM(3, IDENTIFIER)) {
                Node node(N_DEFVAR);
                node.addToken(A(0));
                node.addToken(A(1));
                node.addToken(A(2));
                node.addToken(A(3));
                return node;
            }

            if(len >= 4 && (TM(0, IDENTIFIER) || TM(0, KEYWORD)) && TM(1, IDENTIFIER) && TM(2, EQUAL)) {
                Node node(N_INITVAR);
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
                Node node(N_LOOP);
                int sccount = 0;

                for(Token t : tk)
                    if(t.type == SEMICOLON) sccount++;

                if(sccount == 0) {
                    if(len == 4) {
                        node.addNode(Node(N_COUNT, {}, { A(2) }));
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

            //bool ismatch;
            bool enclosed;
            bool containsParen;
            //int nest;

            /* 括弧チェック */

            while(true) {
                len = tk.size();
                containsParen = false;
                enclosed = true;
                nest = 0;

                for(int i = 0; i < len; i++) {
                    if(match(TA(i), { LPAREN, LBRACK, LANGBLACK, LBRACE })) {
                        containsParen = true;
                        nest++;
                    } else if(match(TA(i), { RPAREN, RBRACK, RANGBLACK, RBRACE })) {
                        containsParen = true;
                        nest--;
                        if(nest == 0 && i != len - 1) enclosed = false; // 括弧に囲まれてない そのまま使える
                    }
                }

                if(tk.at(0).type != LPAREN || tk.at(tk.size() - 1).type != RPAREN)
                    enclosed = false;

                if(enclosed && containsParen && tk.size() >= 3) {
                    std::vector<Token> cutout;  
    
                    for(int i = 1; i < tk.size() - 1; i++)
                        cutout.push_back(tk.at(i));

                    tk = cutout;

                } else break;
            }

            len = tk.size();

            /*std::cout << "tk exp: ";
            for(Token t : tk)
                std::cout << t.string << " ";
            std::cout << std::endl;*/

            /* 論理式 */

            ismatch = false;
            nest = 0;

            for(int i = 0; i < len; i++) {
                if(match(TA(i), { PIPE, AMPERSAND })) {
                    if(nest == 0) ismatch = true;
                } else if(match(TA(i), { LPAREN, LBRACK, LBRACE })) {
                    nest++;
                } else if(match(TA(i), { RPAREN, RBRACK, RBRACE })) {
                    nest--;
                }
            }

            if (len >= 3 && ismatch) {
                Node node(N_LOGIC);
                std::vector<Token> side;
                nest = 0;
                for(int i = 0; i < len; i++) {
                    if(match(TA(i), { PIPE, AMPERSAND }) && nest == 0) {
                        if(side.size() == 1) {node.addNode(Node(N_ITEM, {}, { side.at(0) }));}
                        else node.addNode(getNode(side));
                        side.clear();

                        if(TM(i, PIPE) && TM(i + 1, PIPE)) {
                            node.addNode(Node(N_OPE, {}, { A(i) }));
                            i++;
                        } else if(TM(i, AMPERSAND) && TM(i + 1, AMPERSAND)) {
                            node.addNode(Node(N_OPE, {}, { A(i) }));
                            i++;
                        }
                    } else if(TM(i, LPAREN)) {
                        side.push_back(A(i));
                        nest++;
                    } else if(TM(i, RPAREN)) {
                        side.push_back(A(i));
                        nest--;
                    } else {
                        side.push_back(A(i));
                    }
                }
                if(side.size() == 1) node.addNode(Node(N_ITEM, {}, { side.at(0) }));
                else node.addNode(getNode(side));
                side.clear();
                return node;
            }

            /* 比較式 */

            ismatch = false;
            nest = 0;

            for(int i = 0; i < len; i++) {
                if(match(TA(i), { LANGBLACK, RANGBLACK, EQUAL })) {
                    if(nest == 0) ismatch = true;
                } else if(match(TA(i), { LPAREN, LBRACK, LBRACE })) {
                    nest++;
                } else if(match(TA(i), { RPAREN, RBRACK, RBRACE })) {
                    nest--;
                }
            }

            if (len >= 3 && ismatch) {
                Node node(N_COMP);
                unsigned char type = N_UNKNOWN;
                std::vector<Token> leftside;
                std::vector<Token> rightside;
                for(int i = 0; i < len; i++) {
                    if(TM(i, EQUAL) && TM(i + 1, EQUAL)) {
                        type = N_EQUAL;
                        i++;
                    } else if(TM(i, LANGBLACK)) {
                        type = N_LANGBLACK;
                    } else if(TM(i, RANGBLACK)) {
                        type = N_RANGBLACK;
                    } else if(type == N_UNKNOWN) {
                        leftside.push_back(A(i));
                    } else {
                        rightside.push_back(A(i));
                    }
                }
                node.addNode(Node(type, {}, {}));
                if(leftside.size() == 1) node.addNode(Node(N_ITEM, {}, { leftside.at(0) }));
                else node.addNode(getNode(leftside));
                if(rightside.size() == 1) node.addNode(Node(N_ITEM, {}, { rightside.at(0) }));
                else node.addNode(getNode(rightside));
                return node;
            }

            /* 計算式 */

            ismatch = false;
            nest = 0;

            for(int i = 0; i < len; i++) {
                if(match(TA(i), { TILDE, PLUS, HYPHEN, ASTERISK, SLASH, PERCENTAGE, CARET })) {
                    if(nest == 0) ismatch = true;
                } else if(match(TA(i), { LPAREN, LBRACK, LBRACE })) {
                    nest++;
                } else if(match(TA(i), { RPAREN, RBRACK, RBRACE })) {
                    nest--;
                    if(nest == 0 && i != len - 1) enclosed = false;
                }
            }

            if(len >= 3 && ismatch) {
                Node node(N_EXPRESS);
                std::stack<Token> stack;
                std::vector<Token> item;

                for(int i = 0; i < len; i++) {
                    if(TM(i, LPAREN)) {
                        std::vector<Token> inParen;
                        int nest = 0;
                        for(i++; i < len; i++) {
                            if(TM(i, RPAREN)) {
                                if(nest == 0) {
                                    Node nd = getNode(inParen);
                                    for(Node n : nd.children)
                                        node.addNode(n);
                                    break;
                                } else {
                                    inParen.push_back(A(i));
                                    nest--;
                                }
                            } else if(TM(i, LPAREN)) {
                                inParen.push_back(A(i));
                                nest++;
                            } else {
                                inParen.push_back(A(i));
                            }
                        }
                    } else if(match(TA(i), { TILDE, PLUS, HYPHEN, ASTERISK, SLASH, PERCENTAGE, CARET })) {
                        if(stack.size() == 0) {
                            //スタックの要素数が0 → スタックに積む
                            stack.push(A(i));
                        } else if(compareOpe(stack.top().string, SA(i))) {
                            // 先頭に比べて優先度が高い → スタックに積む
                            stack.push(A(i));
                        } else {
                            // 先頭に比べて優先度が低い → 優先度が低くなるまでaddNode
                            while(stack.size() != 0) {
                                if(compareOpe(SA(i), stack.top().string)) {
                                    node.addNode(Node(N_OPE, {}, { stack.top() }));
                                    stack.pop();
                                } else break;
                            }
                            stack.push(A(i));
                        }
                    } else {
                        for(int j = i; j <= len; j++) {
                            if(j != len && !match(TA(j), { TILDE, PLUS, HYPHEN, ASTERISK, SLASH, PERCENTAGE, CARET })) {
                                item.push_back(A(j));
                            } else {
                                if(j == len) i = j + 1;
                                else i += item.size() - 1;
                                if(item.size() == 1) node.addNode(Node(N_ITEM, {}, { item.at(0) }));
                                else node.addNode(getNode(item));
                                item.clear();
                                break;
                            }
                        }
                    }
                }

                while(stack.size() != 0) {
                    node.addNode(Node(N_OPE, {}, { stack.top() }));
                    stack.pop();
                }

                return node;
            }

        } catch(std::out_of_range ignored) {
            std::cout << "EXCEPTION" << std::endl;
        }

        return Node(N_UNKNOWN);
    }

    // 優先度高い             優先度低い
    // true → ope1 < ope2   false → ope1 >= ope2
    bool compareOpe(std::string ope1, std::string ope2) {

        if(std::regex_match(ope1, std::regex("[~]"))) {
            return (std::regex_match(ope2, std::regex("[+\\-*/]")));
        }

        if(std::regex_match(ope1, std::regex("[+\\-]"))) {
            return (std::regex_match(ope2, std::regex("[*/]")));
        }

        if(std::regex_match(ope1, std::regex("[*/]"))) {
            return (std::regex_match(ope2, std::regex("[]")));
        }

        return false;
    }

    bool match(char type, std::vector<char> matches) {
        bool res = false;
        for(int i = 0; i < matches.size(); i++)
            if(matches[i] == type)
                res = true;
        return res;
    }
};
