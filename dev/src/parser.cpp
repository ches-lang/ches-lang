#pragma once

#include "parser.hpp"



Line::Line() {}

Line::Line(std::vector<Token> tokens) {
    std::vector<Token> line;
    int nest = 0;

    for(int i = 0; i < tokens.size(); i++) {
        if(tokens[i].type == INDENT) {
            nest++;
        } else if(tokens[i].type == COMMENTOUT) {
            continue;
        } else {
            line.push_back(tokens[i]);
        }
    }

    this->tokens = line;
    this->nest = nest;
    this->beginIndex = ((line.size() != 0) ? line[0].index : -1);
}



ParenNest::ParenNest() {}

ParenNest::ParenNest(std::string sourcePath, std::string source) {
    this->sourcePath = sourcePath;
    this->source = source;
}

// 括弧が渡されたら開き括弧と閉じ括弧を判断する
std::vector<Token> ParenNest::getOrderedParens(std::vector<Token> tokens) {
    for(int i = 0; i < tokens.size(); i++) {
        if(tokens[i].match(std::vector<unsigned char> { LPAREN, LBRACK, LBRACE })) {
            this->addOpenParen(tokens[i]);
        } else if(tokens[i].match(std::vector<unsigned char> { RPAREN, RBRACK, RBRACE })) {
            this->addCloseParen(tokens[i]);
        }
    }

    this->checkCloseParensFinally();
    return this->removeSurroundingParens(this->parens);
}

// 閉じ括弧が渡されたら、不正な括弧がないかを確認してネストを書き変える
void ParenNest::addCloseParen(Token token) {
    Token expectingCloseParen = this->latestOpenParen.getOpenParen();
    int nest = this->nestOfParens[this->getNestIndex(token.type)];

    // 階層が0以下なのでエラー
    if(nest <= 0) {
        Console::error("cerr7904", "unexpected closing parenthesis or bracket", { { "at", token.getPositionText(this->sourcePath, this->source) }, { "unexpected", token.string } });
        return;
    }

    // 最後に開いた括弧の種類が異なるのでエラー
    if(token.type == expectingCloseParen.type) {
        Console::error("cerr7904", "unexpected closing parenthesis or bracket", { { "at", token.getPositionText(this->sourcePath, this->source) }, { "unexpected", token.string }, { "expected", expectingCloseParen.string } });
        return;
    }

    this->parens.push_back(token);
    this->nestOfParens[this->getNestIndex(token.type)]--;
}

// 開き括弧が渡されたら、不正な括弧がないかを確認してネストを書き変える
void ParenNest::addOpenParen(Token token) {
    this->latestOpenParen = token;
    this->parens.push_back(token);
    this->nestOfParens[this->getNestIndex(token.type)]++;
}

// 一番最後に閉じられてない括弧がないかチェック
void ParenNest::checkCloseParensFinally() {
    // parensを表示
    /*for(int i = 0; i < parens.size(); i++)
        std::cout<<parens[i].string<<std::endl;*/

    std::string expectedParen = "";

    if(this->nestOfParens[0] > 0)
        expectedParen = ")";
    else if(this->nestOfParens[1] > 0)
        expectedParen = "]";
    else if(this->nestOfParens[2] > 0)
        expectedParen = "}";

    if(expectedParen != "")
        Console::error("cerr5916", "expected closing parenthesis or bracket", { { "expected", expectedParen } });
}

// 外側を取り除いた括弧が不正でないかチェックする
// もし不正ならば取り除いていない状態の括弧を返す
std::vector<Token> ParenNest::removeSurroundingParens(std::vector<Token> tokens) {
    // 渡された括弧の数が1以下ならばtokensを返す
    if(tokens.size() <= 1)
        return tokens;

    std::vector<Token> insideParens;

    // 内側の括弧を取り出す
    for(int i = 1; i < tokens.size() - 1; i++)
        insideParens.push_back(tokens[i]);

    // 内側の括弧の数が1以下ならばtokensを返す
    if(insideParens.size() <= 1)
        return tokens;

    // 最初または最後の括弧が不正ならばtokensを返す
    if(insideParens[0].getOpenParen().type != UNKNOWN
            && insideParens[insideParens.size() - 1].getCloseParen().type != UNKNOWN)
        return tokens;

    // 括弧が不正でなければチェックを繰り返す
    return ParenNest::removeSurroundingParens(insideParens);
}

int ParenNest::getNestIndex(unsigned char type) {
    switch(type) {
        case LPAREN:
        case RPAREN:
        return 0;

        case LBRACK:
        case RBRACK:
        return 1;

        case LBRACE:
        case RBRACE:
        return 2;

        default:
        return -1;
    }
}



Parser::Parser() {}

Parser::Parser(std::string sourcePath, std::string source, std::vector<Token> tokens, Options options) {
    this->sourcePath = sourcePath;
    this->source = source;
    this->tokens = tokens;
    this->lines = this->getLines();
    this->options = options;
}

std::vector<Line> Parser::getLines() {
    std::vector<Line> lines;
    int index = 0;

    while(true) {
        std::vector<Token> ln;

        while(true) {
            Token tk = this->tokens[index];

            if(tk.type == ENDOFFILE || tk.type == NEWLINE) {
                index++;
                break;
            } else {
                ln.push_back(tk);
                index++;
            }
        }

        if(ln.size() > 0)
            lines.push_back(Line(ln));

        if(index >= this->tokens.size())
            break;
    }

    return lines;
}

Node Parser::parse() {
    for(; this->lineIndex < this->lines.size(); this->lineIndex++)
        this->tree.addChild(this->scanNextLine());

    this->tree.print();
    return this->tree;
}

Node Parser::scanNextLine() {
    Node node = this->getNode(CURR_LINE);
    // lineIndexはノード取得後に変更してください
    this->lineIndex++;
    return node;
}

Node Parser::scanNextNest(unsigned char nodeType) {
    Node node(nodeType);
    Line baseLine = CURR_LINE;

    while(true) {
        this->lineIndex++;

        // 行インデックスがLineのサイズを超える場合
        if(this->lineIndex >= this->lines.size())
            break;

        Line currentLine = CURR_LINE;

        // ネストがベース行と同じか、それよりも浅い場合
        if(baseLine.nest >= currentLine.nest) {
            this->lineIndex--;
            break;
        }

        // ネストがチェック中のネストよりも深い場合
        //scanNextLine内でscanNextNestが呼ばれるべき
        // よりネストの深い行はチェックする必要なし

        /*if(baseLine.nest < CURR_LINE.nest)
            if(baseLine.nest < LA(this->lineIndex + 1).nest)
                continue;*/
//std::cout<<baseLine.nest + 1 <<" "<< currentLine.nest<<" "<<currentLine.tokens.at(0).string<<std::endl;
        if(baseLine.nest + 1 < currentLine.nest)
            continue;

        node.addChild(this->getNode(currentLine));
    }

    return node;
}

Node Parser::getNode(Line line, unsigned char defaultType) {
    return this->getNode(line.tokens, line.nest, defaultType);
}

Node Parser::getNode(std::vector<Token> tokens, int nest, unsigned char defaultType) {
    try {
        int len = tokens.size();

        if(len == 0)
            return Node(N_UNKNOWN);

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
                    // todo: コンマを括弧チェック後に変換させる？
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
                node.addChild(this->scanNextNest());
            }

            return node;
        }

        // CALLFUNC
        if(nest >= 1 && len >= 3 && TM(0, IDENTIFIER) && TM(1, LPAREN) && TM(-1, RPAREN)) {
            Node node(N_CALLFUNC);
            node.addToken(A(0));

            int nest = 0;

            if(len >= 4) {
                Node args(N_ARGS);
                std::vector<Token> ag;

                for(int i = 2; i < len - 1; i++) {
                    if(TM(i, COMMA) && nest == 0) {
                        args.addChild(this->getNode(ag));
                        ag.clear();
                    } else if(i == len - 2) {
                        ag.push_back(A(i));
                        args.addChild(getNode(ag));
                    } else {
                        if(A(i).isOpenParen())
                            nest++;
                        else if(A(i).isCloseParen())
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

        // IF
        if(len >= 2 && M(0, KEYWORD, "if")) {
            Node node(N_IF);
            node.addChild(this->getNode(this->copy(1, len - 1, tokens)));
            node.addChild(this->scanNextNest());
            return node;
        }

        // ELSE
        if(len == 1 && M(0, KEYWORD, "else")) {
            Node node(N_ELSE);
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
                    semicolonDiv.push_back({});
                else
                    semicolonDiv[semicolonDiv.size() - 1].push_back(tk);

            if(semicolonDiv.size() == 1) {
                // Example: for(true)
                node.addChild(this->getNode(copy(1, len - 1, tokens), N_LOOP_COND));
            } else if(semicolonDiv.size() == 3) {
                // Example: for(i = 0; i < 5; i++)
                std::vector<Token> item;

                node.addChild(this->getNode(semicolonDiv[0], N_LOOP_INIT));
                node.addChild(this->getNode(semicolonDiv[1], N_LOOP_COND));
                node.addChild(this->getNode(semicolonDiv[2], N_LOOP_CHNG));
            } else {
                //構文エラー
            }

            return node;
        }



        /* 括弧チェック */

        ParenNest parenNest(this->sourcePath, this->source);
        std::vector<Token> orderedTokens = parenNest.getOrderedParens(tokens);

        len = tokens.size();
        bool enclosed;
        bool containsParen;

        /*std::cout << "\ttk exp: ";
        for(Token t : tokens)
            std::cout << t.string << " ";
        std::cout << std::endl;*/

        /* 論理式 */

        Node logicExpNode = this->getLogicalExpressionNode(tokens);

        if(logicExpNode.type != N_UNKNOWN)
            return logicExpNode;

        /* 比較式 */

        Node compExpNode = this->getCompareExpressionNode(tokens);

        if(compExpNode.type != N_UNKNOWN)
            return compExpNode;

        /* 計算式 */



        // トークンの長さが１かどうかは、最後にチェックしてください
        // (最初にチェックすると、else文などが弾かれてしまうため)
        if(len == 1)
            return Node(defaultType, tokens);

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }

    // todo: add an error 'unknown syntax'
    return Node(N_UNKNOWN);
}

// 式でなかった場合のtypeはUNKNOWN
// ここで判断
// 変数'len' および 引数名'tokens' は変更しないでください (A(i)を使用するため)
Node Parser::getLogicalExpressionNode(std::vector<Token> tokens) {
    Node node(N_LOGIC);
    std::vector<Token> side;
    int len = tokens.size();
    int nest = 0;

    for(int i = 0; i < tokens.size(); i++) {
        switch(A(i).type) {
            case AMPERSAND:
            case PIPE:
            if(nest == 0 && i < len - 1 && TA(i) == TA(i + 1)) {
                node.addChild(this->getNode(side, N_ITEM));
                side.clear();

                // TA(i) と TA(i + 1) は同じなので、ここでチェックする必要なし
                if(TM(i, PIPE)) {
                    node.addChild(Node(N_OPE, {}, { A(i) }));
                    i++;
                } else if(TM(i, AMPERSAND)) {
                    node.addChild(Node(N_OPE, {}, { A(i) }));
                    i++;
                }
            }
            break;

            default:
            if(A(i).isOpenParen())
                nest++;
            else if(A(i).isCloseParen())
                nest--;

            side.push_back(A(i));
            break;
        }
    }

    if(node.children.size() > 0)
        return node;
    else
        return Node(N_UNKNOWN);
}

Node Parser::getCompareExpressionNode(std::vector<Token> tokens) {
    Node node(N_COMP);
    unsigned char opeType = N_UNKNOWN;
    std::vector<Token> leftside;
    std::vector<Token> rightside;
    int len = tokens.size();
    int nest = 0;

    for(int i = 0; i < tokens.size(); i++) {//std::cout<<(int)opeType<<" "<<i<<" "<<tokens.size()<<std::endl;
        if(opeType == N_UNKNOWN) {
            // 左辺: まだ比較演算子がきていない
            if(A(i).isOpenParen()) {
                nest++;
            } else if(A(i).isCloseParen()) {
                nest--;
            } else {
                opeType = this->getOpeType(tokens, i);

                switch(opeType) {
                    case N_UNKNOWN:
                    leftside.push_back(A(i));
                    break;

                    case N_LESS:
                    case N_GREATER:
                    break;

                    case N_EQUAL:
                    case N_LESSEQL:
                    case N_GRTREQL:
                    i++;
                    break;
                }
            }
        } else {
            // 右辺: 比較演算子がすでにきた
            rightside.push_back(A(i));
        }
    }

    /*for(Token tk : leftside)
        std::cout<<"lt: "<<tk.string<<std::endl;*/

    /*for(Token tk : rightside)
        std::cout<<"rt: "<<tk.string<<std::endl;*/

    if(leftside.size() == 0 || rightside.size() == 0)
        return Node(N_UNKNOWN);

    node.addChild(this->getNode(leftside));
    node.addChild(this->getNode(rightside));

    if(node.childAt(0).type != N_UNKNOWN && node.childAt(1).type != N_UNKNOWN)
        return Node(N_UNKNOWN);

    return node;
}

// 引数名'tokens' および 変数'len' を変更しないでください (マクロ'A(i)' を使用するため)
unsigned char Parser::getOpeType(std::vector<Token> tokens, int index) {
    int len = tokens.size();
    unsigned char tokenType = TA(index);

    if(index < len - 1) {
        // 2文字の演算子

        unsigned char secondTokenType = TA(index + 1);

        if(tokenType == EQUAL && secondTokenType == EQUAL)
            return N_EQUAL;
        else if(tokenType == LANGBRACK && secondTokenType == EQUAL)
            return N_LESSEQL;
        else if(tokenType == RANGBRACK && secondTokenType == EQUAL)
            return N_GRTREQL;

        // N_EQUALなどの場合は呼び出し元で判断してi++する
    }

    // 1文字の演算子

    if(tokenType == LANGBRACK)
        return N_LESS;
    else if(tokenType == RANGBRACK)
        return N_GREATER;

    // unknownの場合は leftside.push_back(A(i)); をする
    return N_UNKNOWN;
}

/* 過去のコード */

        /* 論理式 */

        /*ismatch = false;
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
        }*/

        /* 比較式 */

        /*ismatch = false;
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
        }*/

        /* 計算式 */

        /*ismatch = false;
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
*/

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
