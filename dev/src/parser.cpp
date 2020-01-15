#pragma once

#include "parser.hpp"



Parser::Parser() {}

Parser::Parser(std::string srcpath, std::string src, std::vector<Token> tk, Options opt) {
    sourcePath = srcpath;
    source = src;
    tokens = tk;
    options = opt;
}

Node Parser::parse() {
    bool b;

    do {
        b = scan();
    } while(b);

    //std::cout << tree.childAt(0).childAt(0).tokens[0].string << std::endl;
    tree.print();
    return tree;
}

bool Parser::scan() {
    if(index >= tokens.size() || tokens[index].type == ENDOFFILE)
        return false;

    indent = 0;
    ln.clear();

    while(true) {
        Token tk = tokens[index];

        if(tk.type == ENDOFFILE) {
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

    if(n.type == N_UNKNOWN)
        return scan();

    tree.addChild(n);
    return true;
}

Node Parser::getNode(std::vector<Token> tk) {
    int len = tk.size();
    bool ismatch = false;
    int nest = 0;

    if(len == 0)
        return Node(N_UNKNOWN);

    if(len == 1)
        return Node(N_TOKEN, tk);

    try {
        std::cout << "tk: ";
        for(Token t : tk)
            std::cout << t.string << " ";
        std::cout << std::endl;

        // DEFFUNC
        if(indent == 0 && TM(0, IDENTIFIER) && TM(1, LPAREN) && TM(len - 1, RPAREN)) {
            Node node(N_DEFFUNC);
            node.addToken(Token(IDENTIFIER, SA(0)));

            if(len >= 4) {
                Node args(N_ARGS);
                std::vector<Token> ag;
                int nest = 0;

                for(int i = 2; i < len - 1; i++) {
                    if(TM(i, COMMA) && nest == 0) {
                        node.addChild(getNode(ag));
                        ag.clear();
                    } else if(i == len - 2) {
                        ag.push_back(A(i));
                    } else {
                        if(A(i).match(std::vector<unsigned char> { LPAREN, LBRACK, LBRACE }))
                            nest++;
                        else if(A(i).match(std::vector<unsigned char> { RPAREN, RBRACK, RBRACE }))
                            nest--;
                        ag.push_back(A(i));
                    }
                }

                node.addChild(args);
            }

            return node;
        }

        // CALLFUNC
        if(nest == 0 && indent >= 1 && len >= 3 && TM(0, IDENTIFIER) && TM(1, LPAREN) && TM(-1, RPAREN)) {
            Node node(N_CALLFUNC);
            node.addToken(Token(IDENTIFIER, SA(0)));

            nest = 0;

            if(len >= 4) {
                Node args(N_ARGS);
                std::vector<Token> ag;
                for(int i = 2; i < len - 1; i++) {
                    if(TM(i, COMMA) && nest == 0) {
                        node.addChild(getNode(ag));
                        ag.clear();
                    } else if(i == len - 2) {
                        ag.push_back(A(i));
                        node.addChild(getNode(ag));
                    } else {
                        if(A(i).match(std::vector<unsigned char> { LPAREN, LBRACK, LBRACE })) {
                            nest++;
                        } else if(A(i).match(std::vector<unsigned char> { RPAREN, RBRACK, LBRACE })) {
                            nest--;
                        }
                        ag.push_back(A(i));
                    }
                }
                node.addChild(args);
            }
            return node;
        }

        // DEFVAR
        if(len == 2 && (TM(0, IDENTIFIER) || TM(0, KEYWORD)) && TM(1, IDENTIFIER)) {
            Node node(N_DEFVAR);
            node.addToken(A(0));
            node.addToken(A(1));
            return node;
        }

        // DEFVAR (ARRAY)
        if(len == 4 && (TM(0, IDENTIFIER) || TM(0, KEYWORD)) && TM(1, LBRACK) && TM(2, RBRACK) && TM(3, IDENTIFIER)) {
            Node node(N_DEFVAR);
            node.addToken(A(0));
            node.addToken(A(1));
            node.addToken(A(2));
            node.addToken(A(3));
            return node;
        }

        // IF
        if(len >= 4 && M(0, KEYWORD, "if") && TM(1, LPAREN) && TM(-1, LPAREN)) {
            Node node(N_IF);
            //node.addChild(getNode(copy(2, len - 3, ln)));
            //if(len == 4) node.addToken(A(2));
            //else node.addChild(getNode(copy(2, len - 3, ln)));
            //Node root(N_ROOT);
            //root.addToken();
            return node;
        }

        // INITVAR
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
                node.addChild(getNode(rs));
            }
            return node;
        }

        // LOOP
        if(len >= 4 && M(0, KEYWORD, "for") && TM(1, LPAREN) && TM(-1, RPAREN)) {
            Node node(N_LOOP);
            int sccount = 0;

            for(Token t : tk)
                if(t.type == SEMICOLON) sccount++;

            if(sccount == 0) {
                if(len == 4) {
                    node.addChild(Node(N_COUNT, {}, { A(2) }));
                } else {
                    std::vector<Token> inParen;
                    for(int i = 2; i < len - 1; i++)
                        inParen.push_back(A(i));
                    node.addChild(getNode(inParen));
                }
            } else if(sccount == 2) {
                std::vector<Token> item;
                for(int i = 2; i < len; i++) {
                    if(TM(i, SEMICOLON)) {
                        node.addChild(getNode(item));
                        item.clear();
                    } else {
                        item.push_back(A(i));
                    }
                }
            }
            return node;
        }

        bool enclosed;
        bool containsParen;

        /* 括弧チェック */

        while(true) {
            len = tk.size();
            containsParen = false;
            enclosed = true;
            nest = 0;

            for(int i = 0; i < len; i++) {
                if(A(i).match(std::vector<unsigned char> { LPAREN, LBRACK, LANGBRACK, LBRACE })) {
                    containsParen = true;
                    nest++;
                } else if(A(i).match(std::vector<unsigned char> { RPAREN, RBRACK, RANGBRACK, RBRACE })) {
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
            if(A(i).match(std::vector<unsigned char> { PIPE, AMPERSAND })) {
                if(nest == 0) ismatch = true;
            } else if(A(i).match(std::vector<unsigned char> { LPAREN, LBRACK, LBRACE })) {
                nest++;
            } else if(A(i).match(std::vector<unsigned char> { RPAREN, RBRACK, RBRACE })) {
                nest--;
            }
        }

        if (len >= 3 && ismatch) {
            Node node(N_LOGIC);
            std::vector<Token> side;
            nest = 0;

            for(int i = 0; i < len; i++) {
                if(A(i).match(std::vector<unsigned char> { PIPE, AMPERSAND }) && nest == 0) {
                    if(side.size() == 1) {node.addChild(Node(N_ITEM, {}, { side.at(0) }));}
                    else node.addChild(getNode(side));
                    side.clear();

                    if(TM(i, PIPE) && TM(i + 1, PIPE)) {
                        node.addChild(Node(N_OPE, {}, { A(i) }));
                        i++;
                    } else if(TM(i, AMPERSAND) && TM(i + 1, AMPERSAND)) {
                        node.addChild(Node(N_OPE, {}, { A(i) }));
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

            if(side.size() == 1) node.addChild(Node(N_ITEM, {}, { side.at(0) }));
            else node.addChild(getNode(side));
            side.clear();
            return node;
        }

        /* 比較式 */

        ismatch = false;
        nest = 0;

        for(int i = 0; i < len; i++) {
            if(A(i).match(std::vector<unsigned char> { LANGBRACK, RANGBRACK, EQUAL })) {
                if(nest == 0) ismatch = true;
            } else if(A(i).match(std::vector<unsigned char> { LPAREN, LBRACK, LBRACE })) {
                nest++;
            } else if(A(i).match(std::vector<unsigned char> { RPAREN, RBRACK, RBRACE })) {
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
                } else if(TM(i, LANGBRACK)) {
                    type = N_LANGBRACK;
                } else if(TM(i, RANGBRACK)) {
                    type = N_RANGBRACK;
                } else if(type == N_UNKNOWN) {
                    leftside.push_back(A(i));
                } else {
                    rightside.push_back(A(i));
                }
            }

            node.addChild(Node(type, {}, {}));
            if(leftside.size() == 1) node.addChild(Node(N_ITEM, {}, { leftside.at(0) }));
            else node.addChild(getNode(leftside));
            if(rightside.size() == 1) node.addChild(Node(N_ITEM, {}, { rightside.at(0) }));
            else node.addChild(getNode(rightside));
            return node;
        }

        /* 計算式 */

        ismatch = false;
        nest = 0;

        for(int i = 0; i < len; i++) {
            if(A(i).match(std::vector<unsigned char> { TILDE, PLUS, HYPHEN, ASTERISK, SLASH, PERCENTAGE, CARET })) {
                if(nest == 0) ismatch = true;
            } else if(A(i).match(std::vector<unsigned char> { LPAREN, LBRACK, LBRACE })) {
                nest++;
            } else if(A(i).match(std::vector<unsigned char> { RPAREN, RBRACK, RBRACE })) {
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
                                    node.addChild(n);
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
                } else if(A(i).match(std::vector<unsigned char> { TILDE, PLUS, HYPHEN, ASTERISK, SLASH, PERCENTAGE, CARET })) {
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
                                node.addChild(Node(N_OPE, {}, { stack.top() }));
                                stack.pop();
                            } else break;
                        }
                        stack.push(A(i));
                    }
                } else {
                    for(int j = i; j <= len; j++) {
                        if(j != len && !A(j).match(std::vector<unsigned char> { TILDE, PLUS, HYPHEN, ASTERISK, SLASH, PERCENTAGE, CARET })) {
                            item.push_back(A(j));
                        } else {
                            if(j == len) i = j + 1;
                            else i += item.size() - 1;
                            if(item.size() == 1) node.addChild(Node(N_ITEM, {}, { item.at(0) }));
                            else node.addChild(getNode(item));
                            item.clear();
                            break;
                        }
                    }
                }
            }

            while(stack.size() != 0) {
                node.addChild(Node(N_OPE, {}, { stack.top() }));
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
bool Parser::compareOpe(std::string ope1, std::string ope2) {
    if(std::regex_match(ope1, std::regex("[~]")))
        return (std::regex_match(ope2, std::regex("[+\\-*/]")));

    if(std::regex_match(ope1, std::regex("[+\\-]")))
        return (std::regex_match(ope2, std::regex("[*/]")));

    if(std::regex_match(ope1, std::regex("[*/]")))
        return (std::regex_match(ope2, std::regex("[]")));

    return false;
}

std::vector<Token> Parser::copy(int begin, int length, std::vector<Token> src) {
    std::vector<Token> res;
    for(int i = begin; i < begin + length; i++)
        res.push_back(src.at(i));
    return res;
}
