#pragma once

#include "parser.hpp"


ches::cmd::Line::Line() {}

ches::cmd::Line::Line(std::vector<Token> tokens) {
    std::vector<Token> line;
    int nest = 0;

    for(int i = 0; i < tokens.size(); i++) {
        if(tokens[i].type == TK_Comment)
            continue;

        line.push_back(tokens[i]);
    }

    this->tokens = line;
    this->beginIndex = ((line.size() != 0) ? line[0].index : -1);
}


ches::cmd::ParenSeq::ParenSeq() {}

ches::cmd::ParenSeq::ParenSeq(std::string sourcePath, std::string source) {
    this->sourcePath = sourcePath;
    this->source = source;
}

// 括弧が渡されたら開き括弧と閉じ括弧を判断する
std::vector<ches::Token> ches::cmd::ParenSeq::getOrderedParens(std::vector<Token> tokens) {
    //int len = tokens.size();
    ByteSeq leftParens = { TK_LeftParen, TK_LeftBracket, TK_LeftBrace };
    ByteSeq rightParens = { TK_RightParen, TK_RightBracket, TK_RightBrace };

    for(int i = 0; i < tokens.size(); i++) {
        if(T_TYPE_MATCH_REG(i, leftParens)) {
            this->addOpenParen(tokens[i]);
        } else if(T_TYPE_MATCH_REG(i, rightParens)) {
            this->addCloseParen(tokens[i]);
        } else if(T_TYPE_MATCH(i, TK_Comma)) {
            std::cout << "..." << std::endl;
        }
    }

    this->checkParensFinally();
    return this->removeSideParens(this->parens);
}

// 閉じ括弧が渡されたら、不正な括弧がないかを確認してネストを書き変える
void ches::cmd::ParenSeq::addCloseParen(Token token) {
    Token expectingCloseParen = this->latestOpenParen.getOpenParen();
    int parenNest = this->parenNest[this->getParenNumber(token.type)];

    // 階層が0以下なのでエラー
    if(parenNest <= 0) {
        Console::log(LogType_Error, 699, { { "At", token.getPositionText(this->sourcePath, this->source) }, { "Unexpected", token.string } });
        return;
    }

    // 最後に開いた括弧の種類が異なるのでエラー
    if(token.type == expectingCloseParen.type) {
        Console::log(LogType_Error, 699, { { "At", token.getPositionText(this->sourcePath, this->source) }, { "Unexpected", token.string }, { "Expected", expectingCloseParen.string } });
        return;
    }

    this->parens.push_back(token);
    int parenNum = this->getParenNumber(token.type);
    this->parenNest[parenNum]--;
}

// 開き括弧が渡されたら、不正な括弧がないかを確認してネストを書き変える
void ches::cmd::ParenSeq::addOpenParen(Token token) {
    this->latestOpenParen = token;
    this->parens.push_back(token);
    int parenNum = this->getParenNumber(token.type);
    this->parenNest[parenNum]++;
}

// 一番最後に閉じられてない括弧がないかチェック
void ches::cmd::ParenSeq::checkParensFinally() {
    // parensを表示
    //for(int i = 0; i < parens.size(); i++)
        //std::cout<<parens[i].string<<std::endl;

    std::string expectedParen = "";

    if(this->parenNest[0] > 0)
        expectedParen = ")";
    else if(this->parenNest[1] > 0)
        expectedParen = "]";
    else if(this->parenNest[2] > 0)
        expectedParen = "}";

    if(expectedParen != "")
        Console::log(LogType_Error, 698, { { "Expected", expectedParen } });
}

// 外側を取り除いた括弧が不正でないかチェックする
// もし不正ならば取り除いていない状態の括弧を返す
std::vector<ches::Token> ches::cmd::ParenSeq::removeSideParens(std::vector<Token> tokens) {
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
    if(insideParens[0].getOpenParen().type != TK_Unknown
            && insideParens[insideParens.size() - 1].getCloseParen().type != TK_Unknown)
        return tokens;

    // 括弧が不正でなければチェックを繰り返す
    return ches::cmd::ParenSeq::removeSideParens(insideParens);
}

int ches::cmd::ParenSeq::getParenNumber(Byte type) {
    switch(type) {
        case TK_LeftParen:
        case TK_RightParen:
        return 0;

        case TK_LeftBracket:
        case TK_RightBracket:
        return 1;

        case TK_LeftBrace:
        case TK_RightBrace:
        return 2;

        default:
        return -1;
    }
}


ches::cmd::Parser::Parser() {}

ches::cmd::Parser::Parser(std::string sourcePath, std::string source, std::vector<Token> tokens) {
    this->sourcePath = sourcePath;
    this->source = source;
    this->tokens = tokens;
    this->lines = this->getLines();
}

std::vector<ches::cmd::Line> ches::cmd::Parser::getLines() {
    std::vector<Line> lines;
    int tokenIndex = 0;

    while(true) {
        std::vector<Token> line;

        while(true) {
            Token token = this->tokens.at(tokenIndex);
            tokenIndex++;

            if(token.type == TK_EndOfFile)
                break;

            if(token.type == TK_NewLine) {
                if(line.size() == 0)
                    line.push_back(token);

                break;
            }

            line.push_back(token);
        }

        if(line.size() > 0)
            lines.push_back(Line(line));

        if(tokenIndex >= this->tokens.size())
            break;
    }

    return lines;
}

ches::Node ches::cmd::Parser::parse() {
    while(this->lineIndex == -1 || this->lineIndex + 1 < this->lines.size())
        this->tree.addChild(this->scanNextLine());

    // ブロックが開いたままである場合はエラー
    int blockNestDiff = std::abs(0 - this->blockNest);

    while(blockNestDiff > 0) {
        Console::log(LogType_Error, 4952, { { "At", T_AT(-1).getPositionText(this->sourcePath, this->source) }, { "Expected", "end" } });
        blockNestDiff--;
    }

    this->tree.print();///
    return this->tree;
}

ches::Node ches::cmd::Parser::scanNextLine() {
    this->lineIndex++;
    Node node = this->getNode(CURR_LINE.tokens, ND_Root);
    return node;
}

ches::Node ches::cmd::Parser::scanNextNest(Byte nodeType) {
    Node n_root(nodeType);
    Line baseLine = CURR_LINE;

    this->blockNest++;

    while(true) {
        this->lineIndex++;

        // 行インデックスがLineのサイズを超える場合
        if(this->lineIndex >= this->lines.size())
            break;

        std::vector<Token> tokens = CURR_LINE.tokens;

        // std::cout << "    ln(" << this->lineIndex << ";" << "n" << this->blockNest << "):   ";
        // for(Token tk : tokens)
        //     std::cout << (tk.string == "\n" ? "\\n" : tk.string) << " ";
        // std::cout << std::endl;///

        // ブロックの終わりが来たらbreakする
        if(T_MATCH(0, TK_Keyword, "end") ||
                T_MATCH(0, TK_Keyword, "elif") ||
                T_MATCH(0, TK_Keyword, "else")) {
            this->blockNest--;
            break;
        }

        n_root.addChild(this->getNode(CURR_LINE.tokens, ND_Root));
    }

    return n_root;
}

ches::Node ches::cmd::Parser::getNode(std::vector<Token> tokens, Byte defaultType) {
    try {
        int len = tokens.size();

        if(len == 0)
            return Node(ND_Unknown);

        std::cout << "tk(" << this->lineIndex << ";n" + std::to_string(this->blockNest) + "): ";
        for(Token tk : tokens)
            std::cout << (tk.string == "\n" ? "\\n" : tk.string) << " ";
        std::cout << std::endl;///

        // DEFCLASSNAME
        // todo: マッチの条件を変える (名前空間などに対応)
        if(this->className == "" && this->blockNest == 0 && T_TYPE_MATCH(0, TK_Identifier)) {
            this->className = T_AT(0).string;
            return this->scanNextLine();
        }

        // クラス名が定義されていない場合はエラー
        if(this->className == "")
            Console::log(LogType_Error, 4952, { { "At", T_AT(0).getPositionText(this->sourcePath, this->source) }, { "Expected", "<class-name>" } }, true);

        // 改行は無視する (行数保持のために改行文字のみの行が残してある)
        if(T_TYPE_MATCH(0, TK_NewLine)) {
            return this->scanNextLine();
        }

        // endキーワードはscanNextNest()内で処理される
        // ブロック外でendキーワードが来た場合はエラー
        if(T_MATCH(0, TK_Keyword, "end"))
            Console::log(LogType_Error, 699, { { "At", T_AT(0).getPositionText(this->sourcePath, this->source) }, { "Unexpected", "end" } });

        // DEFFUNC
        if(this->blockNest == 0 && T_TYPE_MATCH(0, TK_Identifier) && T_TYPE_MATCH(1, TK_LeftParen) && T_TYPE_MATCH(-1, TK_RightParen)) {
            Node n_root(ND_DefFunc);
            Node n_args(ND_Args);
            n_root.addToken(Token(TK_Identifier, T_AT(0).string));

            if(len >= 4) {
                std::vector<Token> args;
                int nestInArgs = 0;

                for(int i = 2; i < len - 1; i++) {
                    // todo: コンマを括弧チェック後に変換させる？
                    if(T_TYPE_MATCH(i, TK_Comma) && nestInArgs == 0) {
                        n_args.addChild(this->getNode(args));
                        args.clear();
                    } else if(i == len - 2) {
                        args.push_back(T_AT(i));
                        n_args.addChild(this->getNode(args));
                    } else {
                        if(T_AT(i).match(ByteSeq { TK_LeftParen, TK_LeftBracket, TK_LeftBrace }))
                            nestInArgs++;
                        else if(T_AT(i).match(ByteSeq { TK_RightParen, TK_RightBracket, TK_RightBrace }))
                            nestInArgs--;
                        args.push_back(T_AT(i));
                    }
                }
            }

            n_root.addChild(n_args);
            n_root.addChild(this->scanNextNest());
            return n_root;
        }

        // CALLFUNC
        if(this->blockNest >= 1 && len >= 3 && T_TYPE_MATCH(0, TK_Identifier) && T_TYPE_MATCH(1, TK_LeftParen) && T_TYPE_MATCH(-1, TK_RightParen)) {
            Node n_root(ND_CallFunc);
            Node n_args(ND_Args);
            n_root.addToken(T_AT(0));

            if(len >= 4) {
                std::vector<Token> args;
                int parenNest = 0;

                for(int i = 2; i < len - 1; i++) {
                    if(T_TYPE_MATCH(i, TK_Comma) && parenNest == 0) {
                        n_args.addChild(this->getNode(args));
                        args.clear();
                    } else if(i == len - 2) {
                        args.push_back(T_AT(i));
                        n_args.addChild(getNode(args));
                    } else {
                        if(T_AT(i).isOpenParen())
                            parenNest++;
                        else if(T_AT(i).isCloseParen())
                            parenNest--;

                        args.push_back(T_AT(i));
                    }
                }
            }

            n_root.addChild(n_args);
            return n_root;
        }

        // DEFVAR
        if(len == 2 && (T_TYPE_MATCH(0, TK_Identifier) || T_TYPE_MATCH(0, TK_Keyword)) && T_TYPE_MATCH(1, TK_Identifier)) {
            Node n_root(ND_DefVar);
            n_root.addToken(T_AT(0));
            n_root.addToken(T_AT(1));
            return n_root;
        }

        // DEFVAR (ARRAY)
        if(len == 4 && (T_TYPE_MATCH(0, TK_Identifier) || T_TYPE_MATCH(0, TK_Keyword)) && T_TYPE_MATCH(1, TK_LeftBracket) && T_TYPE_MATCH(2, TK_RightBracket) && T_TYPE_MATCH(3, TK_Identifier)) {
            Node n_root(ND_DefVar);
            n_root.addToken(T_AT(0));
            n_root.addToken(T_AT(1));
            n_root.addToken(T_AT(2));
            n_root.addToken(T_AT(3));
            return n_root;
        }

        // IF
        if(len >= 2 && T_MATCH(0, TK_Keyword, "if")) {
            Node n_root(ND_If);
            n_root.addChild(this->getNode(this->copy(1, len - 1, tokens)));
            n_root.addChild(this->scanNextNest());

            while(CURR_LINE.tokens.size() == 1 && CURR_LINE.tokens.at(0).type == TK_Keyword && CURR_LINE.tokens.at(0).string == "elif")
                n_root.addChild(this->scanNextNest(ND_ElseIf));

            if(CURR_LINE.tokens.size() == 1 && CURR_LINE.tokens.at(0).type == TK_Keyword && CURR_LINE.tokens.at(0).string == "else")
                n_root.addChild(this->scanNextNest(ND_Else));

            return n_root;
        }

        // ELSE
        if(len == 1 && T_MATCH(0, TK_Keyword, "else")) {
            Console::log(LogType_Error, 699, { { "At", T_AT(0).getPositionText(this->sourcePath, this->source) }, { "Unexpected", T_AT(0).string } });
        }

        // ELSEIF
        if(len == 1 && T_MATCH(0, TK_Keyword, "elif")) {
            Console::log(LogType_Error, 699, { { "At", T_AT(0).getPositionText(this->sourcePath, this->source) }, { "Unexpected", T_AT(0).string } });
        }

        // INITVAR todo: 複数トークンの型名 (e.g. ジェネリック) にも対応させる
        if(len >= 4 && (T_TYPE_MATCH(0, TK_Identifier) || T_TYPE_MATCH(0, TK_Keyword)) && T_TYPE_MATCH(1, TK_Identifier) && T_TYPE_MATCH(2, TK_Equal)) {
            Node n_root(ND_InitVar);
            n_root.addToken(T_AT(0));
            n_root.addToken(T_AT(1));
            n_root.addChild(this->getNode(this->copy(3, len - 3, tokens)));

            /*if(len == 4) {
                n_root.addToken(T_AT(3));
            } else {
                std::vector<Token> rs;
                for(int i = 3; i < len; i++)
                    rs.push_back(T_AT(i));
                n_root.addChild(getNode(rs));
            }*/

            return n_root;
        }

        // LOOP
        if(len >= 4 && T_MATCH(0, TK_Keyword, "for") && T_TYPE_MATCH(1, TK_LeftParen) && T_TYPE_MATCH(-1, TK_RightParen)) {
            Node n_root(ND_Loop);
            std::vector<std::vector<Token>> semicolonDiv = {};

            for(Token tk : tokens)
                if(tk.type == TK_Semicolon)
                    semicolonDiv.push_back({});
                else
                    semicolonDiv[semicolonDiv.size() - 1].push_back(tk);

            if(semicolonDiv.size() == 1) {
                // Example: for(true)
                n_root.addChild(this->getNode(this->copy(1, len - 1, tokens), ND_Loop_Cond));
            } else if(semicolonDiv.size() == 3) {
                // Example: for(i = 0; i < 5; i++)
                std::vector<Token> item;

                n_root.addChild(this->getNode(semicolonDiv[0], ND_Loop_Init));
                n_root.addChild(this->getNode(semicolonDiv[1], ND_Loop_Cond));
                n_root.addChild(this->getNode(semicolonDiv[2], ND_Loop_Change));
            } else {
                //構文エラー
            }

            return n_root;
        }



        /* 括弧チェック */

        ParenSeq parenSeq(this->sourcePath, this->source);
        std::vector<Token> orderedTokens = parenSeq.getOrderedParens(tokens);

        len = tokens.size();
        bool enclosed;
        bool containsParen;

        /*std::cout << "\ttk exp: ";
        for(Token t : tokens)
            std::cout << t.string << " ";
        std::cout << std::endl;*/

        /* 論理式 */

        Node n_logicExp = this->getLogicalExpressionNode(tokens);

        if(n_logicExp.type != ND_Unknown)
            return n_logicExp;

        /* 比較式 */

        Node n_compExp = this->getCompareExpressionNode(tokens);

        if(n_compExp.type != ND_Unknown)
            return n_compExp;

        /* 計算式 */



        // トークンの長さが１かどうかは、最後にチェックしてください
        // (最初にチェックすると、else文などが弾かれてしまうため)
        if(len == 1)
            return Node(defaultType, tokens);

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }

    Console::log(LogType_Error, 9034, { { "At", T_AT(0).getPositionText(this->sourcePath, this->source) } });
    return Node(ND_Unknown);
}

// 式でなかった場合のtypeはUNKNOWN
// ここで判断
// 変数'len' および 引数名'tokens' は変更しないでください (T_AT(i)を使用するため)
ches::Node ches::cmd::Parser::getLogicalExpressionNode(std::vector<Token> tokens) {
    Node node(ND_Logic);
    std::vector<Token> side;
    int len = tokens.size();
    int nest = 0;

    for(int i = 0; i < tokens.size(); i++) {
        switch(T_AT(i).type) {
            case TK_Ampersand:
            case TK_Pipe:
            if(nest == 0 && i < len - 1 && T_AT(i).type == T_AT(i + 1).type) {
                node.addChild(this->getNode(side, ND_Root)); // N_ITEM
                side.clear();

                // T_AT(i).type と T_AT(i + 1).type は同じなので、ここでチェックする必要なし
                if(T_TYPE_MATCH(i, TK_Pipe)) {
                    node.addChild(Node(ND_Operator, {}, { T_AT(i) }));
                    i++;
                } else if(T_TYPE_MATCH(i, TK_Ampersand)) {
                    node.addChild(Node(ND_Operator, {}, { T_AT(i) }));
                    i++;
                }
            }
            break;

            default:
            if(T_AT(i).isOpenParen())
                nest++;
            else if(T_AT(i).isCloseParen())
                nest--;

            side.push_back(T_AT(i));
            break;
        }
    }

    if(node.children.size() > 0)
        return node;
    else
        return Node(ND_Unknown);
}

ches::Node ches::cmd::Parser::getCompareExpressionNode(std::vector<Token> tokens) {
    Node node(ND_Compare);
    Byte opeType = ND_Unknown;
    std::vector<Token> leftside;
    std::vector<Token> rightside;
    int len = tokens.size();
    int nest = 0;

    for(int i = 0; i < tokens.size(); i++) {//std::cout<<(int)opeType<<" "<<i<<" "<<tokens.size()<<std::endl;
        if(opeType == ND_Unknown) {
            // 左辺: まだ比較演算子がきていない
            if(T_AT(i).isOpenParen()) {
                nest++;
            } else if(T_AT(i).isCloseParen()) {
                nest--;
            } else {
                opeType = this->getOpeType(tokens, i);

                switch(opeType) {
                    case ND_Unknown:
                    leftside.push_back(T_AT(i));
                    break;

                    case ND_Less:
                    case ND_Greater:
                    break;

                    case ND_Equal:
                    case ND_LessEqual:
                    case ND_GreaterEqual:
                    i++;
                    break;
                }
            }
        } else {
            // 右辺: 比較演算子がすでにきた
            rightside.push_back(T_AT(i));
        }
    }

    /*for(Token tk : leftside)
        std::cout<<"lt: "<<tk.string<<std::endl;*/

    /*for(Token tk : rightside)
        std::cout<<"rt: "<<tk.string<<std::endl;*/

    if(leftside.size() == 0 || rightside.size() == 0)
        return Node(ND_Unknown);

    node.addChild(this->getNode(leftside));
    node.addChild(this->getNode(rightside));

    if(node.childAt(0).type != ND_Unknown && node.childAt(1).type != ND_Unknown)
        return Node(ND_Unknown);

    return node;
}

// 引数名'tokens' および 変数'len' を変更しないでください (マクロ'T_AT(i)' を使用するため)
ches::Byte ches::cmd::Parser::getOpeType(std::vector<Token> tokens, int index) {
    int len = tokens.size();
    Byte tokenType = T_AT(index).type;

    if(index < len - 1) {
        // 2文字の演算子

        Byte secondTokenType = T_AT(index + 1).type;

        if(tokenType == TK_Equal && secondTokenType == TK_Equal)
            return ND_Equal;
        else if(tokenType == TK_LeftAngleBracket && secondTokenType == TK_Equal)
            return ND_LessEqual;
        else if(tokenType == TK_RightAngleBracket && secondTokenType == TK_Equal)
            return ND_GreaterEqual;

        // N_EQUALなどの場合は呼び出し元で判断してi++する
    }

    // 1文字の演算子

    if(tokenType == TK_LeftAngleBracket)
        return ND_Less;
    else if(tokenType == TK_RightAngleBracket)
        return ND_Greater;

    // unknownの場合は leftside.push_back(T_AT(i)); をする
    return ND_Unknown;
}

// 優先度高い             優先度低い
// true → ope1 < ope2   false → ope1 >= ope2
bool ches::cmd::Parser::compareOpe(std::string ope1, std::string ope2) {
    if(std::regex_match(ope1, std::regex("[~]")))
        return (std::regex_match(ope2, std::regex("[+\\-*/]")));

    if(std::regex_match(ope1, std::regex("[+\\-]")))
        return (std::regex_match(ope2, std::regex("[*/]")));

    if(std::regex_match(ope1, std::regex("[*/]")))
        return (std::regex_match(ope2, std::regex("[]")));

    return false;
}

std::vector<ches::Token> ches::cmd::Parser::copy(int begin, int length, std::vector<Token> src) {
    std::vector<Token> res;

    for(int i = begin; i < begin + length; i++)
        res.push_back(src.at(i));

    return res;
}
