#pragma once

#include "parser.hpp"



Line::Line() {}

Line::Line(std::vector<Token> tokens, int beginIndex, int endIndex, int nest) {
    this->tokens = tokens;
    this->nest = nest;
    this->beginIndex = beginIndex;
    this->endIndex = endIndex;
}



ParenNest::ParenNest() {}

// 括弧が渡されたら、不正な括弧がないかを確認してネストを書き変える
// エラーが起きたかを返す
bool ParenNest::checkParen(Token token) {
    int *nest = this->getNest(token.type);

    if(token.match(std::vector<unsigned char> { LPAREN, LBRACK, LANGBRACK, LBRACE })) {
        nest++;
        return;
    }

    /*
    [([])]
    [(])
    >{}
    
    */

    if(token.match(std::vector<unsigned char> { RPAREN, RBRACK, RANGBRACK, RBRACE })) {
        // 階層が0未満なのでエラー
        if(nest < 0) {
            return;
        }

        // 最後に開いた括弧の種類が異なるのでエラー
        if(this->latestOpenParen.type == token.type) {
            return;
        }

        nest--;

        return;
    }
}

int* ParenNest::getNest(unsigned char type) {
    switch(type) {
        case LPAREN:
        case RPAREN:
        return this->paren;

        case LBRACK:
        case RBRACK:
        return this->brack;

        case LANGBRACK:
        case RANGBRACK:
        return this->angbrack;

        case LBRACE:
        case RBRACE:
        return this->brace;

        default:
        return (int*)0;
    }
}



Parser::Parser() {}

Parser::Parser(std::string srcpath, std::string src, std::vector<Token> tk, Options opt) {
    sourcePath = srcpath;
    source = src;
    tokens = tk;
    this->lines = this->getLines();
    options = opt;
}

Node Parser::parse() {
    int tklen = tokens.size();

    do {
        tree.addChild(scanNextLine());
    } while(tokens[index].type != ENDOFFILE);

    tree.print();
    return tree;
}

std::vector<Line> Parser::getLines() {
    std::vector<Line> lines;
    int index = 0;

    while(true) {
        std::vector<Token> line;
        int indent = 0;

        while(true) {
            Token tk = this->tokens[index];

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
                line.push_back(tk);
                index++;
            }
        }

        lines.push_back(Line(ln));

        if(index >= this->tokens.size())
            break;
    }

    return lines;
}

Node Parser::scanNextLine() {std::cout<<"scanNextLine"<<std::endl;
    this->lineIndex++;
    return this->getNode(LA(this->lineIndex));
}

Node Parser::scanNextNest(unsigned char nodeType) {
    Node node(nodeType);
    Line baseLine = LA(this->lineIndex);

    while(true) {
        Line currentLine = LA(this->lineIndex);

        // 行インデックスがトークン列のサイズを超える場合
        if(this->lineIndex >= this->tokens.size())
            break;

        // ネストがベース行よりも浅い場合
        if(baseLine.indent >= currentLine.indent)
            break;

        // ネストがベース行よりも深い場合
        if(this->lineIndex < this->tokens.size() - 1)
            if(baseLine.indent < LA(this->lineIndex + 1).indent)
                std::cout << "nest error" << std::endl;
                //scanNextLine内でscanNextNestが呼ばれるべき
                //error

        node.addChild(this->getNode(currentLine));
    }

    return node;
}

Node Parser::getNode(Line line, unsigned char defaultType) {
    return this->getNode(line.tokens, defaultType);
}

Node Parser::getNode(std::vector<Token> tokens, int nest, unsigned char defaultType) {
    int len = tokens.size();
    bool isLine
    //bool ismatch = false;
    //int nest = 0;

    if(len == 0)
        return Node(N_UNKNOWN);

    if(len == 1)
        return Node(defaultType, tokens);

    try {
        std::cout << "tk: ";
        for(Token tk : tokens)
            std::cout << tk.string << " ";
        std::cout << std::endl;

        // DEFFUNC
        if(nest == 0 && TM(0, IDENTIFIER) && TM(1, LPAREN) && TM(len - 1, RPAREN)) {
            Node node(N_DEFFUNC);
            node.addToken(Token(IDENTIFIER, SA(0)));

            if(len >= 4) {
                Node args(N_ARGS);
                std::vector<Token> ag;
                int nestInArgs = 0;

                for(int i = 2; i < len - 1; i++) {
                    if(TM(i, COMMA) && nestInArgs == 0) {
                        node.addChild(this->getNode(ag));
                        ag.clear();
                    } else if(i == len - 2) {
                        ag.push_back(A(i));
                    } else {
                        if(A(i).match(std::vector<unsigned char> { LPAREN, LBRACK, LBRACE }))
                            nestInArgs++;
                        else if(A(i).match(std::vector<unsigned char> { RPAREN, RBRACK, RBRACE }))
                            nestInArgs--;
                        ag.push_back(A(i));
                    }
                }

                node.addChild(args);
            }

            return node;
        }

        // CALLFUNC
        if(nest == 1 && len >= 3 && TM(0, IDENTIFIER) && TM(1, LPAREN) && TM(-1, RPAREN)) {
            Node node(N_CALLFUNC);
            node.addToken(Token(IDENTIFIER, SA(0)));

            int nest = 0;

            if(len >= 4) {
                Node args(N_ARGS);
                std::vector<Token> ag;

                for(int i = 2; i < len - 1; i++) {
                    if(TM(i, COMMA) && nest == 0) {
                        node.addChild(this->getNode(ag));
                        ag.clear();
                    } else if(i == len - 2) {
                        ag.push_back(A(i));
                        node.addChild(getNode(ag));
                    } else {
                        if(A(i).match(std::vector<unsigned char> { LPAREN, LBRACK, LBRACE }))
                            nest++;
                        else if(A(i).match(std::vector<unsigned char> { RPAREN, RBRACK, LBRACE }))
                            nest--;

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

        // IF memo: changed IF statement syntax
        if(len >= 2 && M(0, KEYWORD, "if")) {
            Node node(N_IF);
            node.addChild(getNode(copy(1, len - 1, tokens)));

            //Node root(N_ROOT);

            /*int nextLineIndex = index;

            do {std::cout<<"getNode"<<std::endl;
                NextLine nextLine = getLine(nextLineIndex);

                std::cout<<"aaa: "<<nextLine.nextStartIndex<<" "<<std::hex<<(int)tokens[nextLine.nextStartIndex].type<<std::endl;

                if(tk.size() == 0)
                    continue;

                if(indent < nextLine.indent)
                    root.addChild(scanNextLine());
                else break;
            } while(tokens[index].type != ENDOFFILE);*/

            node.addChild(this->scanNextNest());
            return node;
        }

        // INITVAR todo: 複数トークンの型名にも対応させる
        if(len >= 4 && (TM(0, IDENTIFIER) || TM(0, KEYWORD)) && TM(1, IDENTIFIER) && TM(2, EQUAL)) {
            Node node(N_INITVAR);
            node.addToken(A(0));
            node.addToken(A(1));

            node.addChild(this->getNode(copy(3, len - 1, tokens)));

            /*if(len == 4) {
                node.addToken(A(3));
            } else {
                std::vector<Token> rs;
                for(int i = 3; i < len; i++)
                    rs.push_back(A(i));
                node.addChild(getNode(rs));
            }*/

            return node;
        }

        // LOOP
        if(len >= 4 && M(0, KEYWORD, "for") && TM(1, LPAREN) && TM(-1, RPAREN)) {
            Node node(N_LOOP);
            std::vector<std::vector<Token>> semicolonDiv = {};

            for(Token tk : tokens)
                if(tk.type == SEMICOLON)
                    div.push_back({});
                else
                    div[div.size() - 1].push_back(tk);

            if(semicolonDiv.size() == 1) {
                // Example: for(true)
                node.addChild(this->getNode(copy(1, len - 1, tokens), N_LOOP_COND));
            } else if(semicolonDiv.size() == 3) {
                // Example: for(i = 0; i < 5; i++)
                std::vector<Token> item;

                node.addChild(this->getNode(semicolonDiv[0], N_LOOP_INIT));
                node.addChild(this->getNode(semicolonDiv[1], N_LOOP_COND));
                node.addChild(this->getNode(semicolonDiv[2], N_LOOP_CHANGE));
            } else {
                //構文エラー
            }

            return node;
        }



        tokens = this->removeParensAtBothEnd();

        bool enclosed;
        bool containsParen;

        /* 括弧チェック */

        

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
                    node.addChild(getNode(side, N_ITEM));
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

            node.addChild(getNode(side, N_ITEM));
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

        if(len >= 3 && ismatch) {
            Node node(N_COMP);
            unsigned char type = N_UNKNOWN;
            std::vector<Token> leftside;
            std::vector<Token> rightside;

            for(int i = 0; i < len; i++) {
                if(type == N_UNKNOWN) {
                    // 左辺: まだ比較演算子がきていない
                    if(i < len - 1 && TM(i, EQUAL) && TM(i + 1, EQUAL)) {
                        type = N_EQUAL;
                        i++;
                    } else if(TM(i, LANGBRACK)) {
                        type = N_LESS;
                    } else if(TM(i, RANGBRACK)) {
                        type = N_GREATER;
                    } else {
                        leftside.push_back(A(i));
                    }
                } else {
                    // 右辺: 比較演算子がすでにきた
                    rightside.push_back(A(i));
                }
            }

            node.addChild(Node(type, {}, {}));
            node.addChild(getNode(leftside, N_ITEM));
            node.addChild(getNode(rightside, N_ITEM));

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

    // todo: add an error'unknown syntax'
    return Node(N_UNKNOWN);
}

// 不正な括弧がないかをチェックする
// 両端の括弧を削除する必要があるかを返す
bool Parser::checkParensAtBothEnd(std::vector<Token> tokens) {
    ParenNest parenNest;

    for(int i = 0; i < len; i++)
        parenNest.checkToken(A(i));

    return;
}

std::vector<Token> Parser::removeParensAtBothEnd(std::vector<Token> tokens) {
    std::vector<Token> resTokens;

    if(this->checkParensAtBothEnd(tokens))
        return resTokens;

    int len = tokens.size();
    int parenCountAtBothEnd = 0;
    int parenNest = 0;

    for(int i = 0; i < len; i++) {
        switch(tokens[i].type) {
            case LPAREN:
            parenNest++;
            
            resTokens.push_back(tokens[i]);
            break;

            case RPAREN:
            parenNest--;
            resTokens.push_back(tokens[i]);
            break;

            default:
            resTokens.push_back(tokens[i]);
            break;
        }
    }

    /*int len = tokens.size();
    bool containsParen;
    bool enclosed;

    while(true) {
        int nest = 0;
        containsParen = false;
        enclosed = true;

        for(int i = 0; i < len; i++) {
            if(A(i).match(std::vector<unsigned char> { LPAREN, LBRACK, LANGBRACK, LBRACE })) {
                containsParen = true;
                nest++;
            } else if(A(i).match(std::vector<unsigned char> { RPAREN, RBRACK, RANGBRACK, RBRACE })) {
                containsParen = true;
                nest--;

                // 括弧に囲まれてないのでそのまま使える(？)
                if(nest == 0 && i != len - 1)
                    enclosed = false;
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
        }*/

    return resTokens;
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

// int: 結果の次行の開始インデックス | vector<Token>: 結果行のトークン配列
NextLine Parser::getLine(int startIndex) {
    std::vector<Token> ln;
    int nextLineIndex = startIndex;
    int indent;

    for(int i = startIndex; i < tokens.size(); i++, nextLineIndex++) {
        Token tk = tokens[i];
        std::cout<<std::hex<<(int)tk.type<<" ";
        if(tk.type == ENDOFFILE) {
            break;
        } else if(tk.type == NEWLINE) {
            break;
        } else if(tk.type == INDENT) {
            indent++;
            continue;
        } else if(tk.type == COMMENTOUT) {
            continue;
        } else {
            ln.push_back(tk);std::cout<<tk.string<<" | ";
        }
    }std::cout<<std::endl;

    nextLineIndex++;
    std::cout<<"aaaaaa: "<<std::hex<<(int)tokens[nextLineIndex].type<<std::endl;
    return NextLine(ln, startIndex, nextLineIndex, indent);
    //return std::pair(nextLineIndex, ln);
}
