#pragma once

#define MATCH_STR(ch1)                  (this->source[index] == ch1)
#define MATCH_STR_2(ch1, ch2)           (index < this->source.length() - 1 && this->source[index] == ch1 && this->source[index + 1] == ch2)
#define MATCH_STR_3(ch1, ch2, ch3)      (index < this->source.length() - 3 && this->source[index] == ch1 && this->source[index + 1] == ch2 && this->source[index + 2] == ch3)
#define MATCH_STR_4(ch1, ch2, ch3, ch4) (index < this->source.length() - 3 && this->source[index] == ch1 && this->source[index + 1] == ch2 && this->source[index + 2] == ch3 && this->source[index + 3] == ch4)

#define MATCH_STR_M1(ch1)               (index > 0 && this->source[index - 1] == ch1)

#define GET_TOKEN(type)                 Token(type, std::string { this->source[index] }, index)
#define JUDGE_TOKEN(char, type)         if(MATCH_STR(char)) return GET_TOKEN(type);
#define JUDGE_PAREN_TOKEN(char, type)   if(MATCH_STR(char)) { Token token = GET_TOKEN(type); validateParen(token); return token; }

#include "lexer.cpp"


ches::cmd::Lexer::Lexer() {}

ches::cmd::Lexer::Lexer(std::string filePath, std::string &source) {
    this->filePath = filePath;
    this->source = source;
}

ches::TokenSeq ches::cmd::Lexer::splitTokens() {
    TokenSeq tokenList;
    Token token;

    do {
        token = getNextToken();
        tokenList.push_back(token);
        std::cout << (int)token.type << "\t" << ((token.string == "\n") ? "\\n" : token.string) << std::endl;///
    } while(token.type != TK_EndOfFile);

    // 括弧が閉じられていない場合はまとめてエラーを出力
    for(Token paren : this->openParens) {
        std::string position = Token::getPositionText(g_cmd.argKeyAt("-i"), this->source, paren.index + 1);
        std::string expected = "'" + paren.getCloseParen().string + "'";

        Console::log(LogType_Error, 698, { { "At", position }, { "Expected", expected } });
    }

    if(Console::errored || (Console::warned && !g_cmd.existsArgKey("-miss")))
        exit(-1);

    return tokenList;
}

ches::Token ches::cmd::Lexer::getNextToken() {
    index++;

    if(index >= this->source.size())
        return Token(TK_EndOfFile, "", index);

    if(MATCH_STR_2('*', '/'))
        Console::log(LogType_Error, 699, { { "At", Token::getPositionText(g_cmd.argKeyAt("-i"), this->source, index++) }, { "Unexpected", "*/" } });

    if(MATCH_STR_2('/', '*')) {
        std::string comment;
        int startIndex = index;

        for(index += 2; index < this->source.length() - 1; index++) {
            if(this->source[index] == '*' && this->source[index + 1] == '/') {
                index++;
                return Token(TK_Comment, comment, startIndex);
            }

            comment += std::string { this->source[index] };
        }

        //checkParenFinally();

        // コメントアウトに閉じがない場合はエラー
        Console::log(LogType_Error, 1562, { { "At", Token::getPositionText(g_cmd.argKeyAt("-i"), this->source, startIndex + 2) }, { "Expected", "'*/'" } }, true);
    }

    else if(MATCH_STR_2('/', '/')) {
        std::string comment;
        int startIndex = index;

        for(index++; index < this->source.length(); index++) {
            if(this->source[index] == '\n') {
                index--;
                break;
            }

            comment += std::string { this->source[index] };
        }

        return Token(TK_Comment, comment, startIndex);
    }

    // 基本的なトークンを判定する
    for(token_pair pair : tokenPairs)
        JUDGE_TOKEN(pair.first, pair.second);

    // 括弧トークンを判定する
    for(token_pair pair : parenTokenPairs)
        JUDGE_TOKEN(pair.first, pair.second);

    if(MATCH_STR(' ')) {
        // インデントでない場合は次のトークンを返す
        for(int i = index - 1; i >= 0; i--) {
            if(this->source[i] == ' ') {
                continue;
            } else {
                return getNextToken();
            }
        }
    }

    else if(MATCH_STR('\n')) {
        // for(index++; index < this->source.length(); index++) {
        //     if(!MATCH_STR('\n')) {
        //         index--;
        //         break;
        //     }
        // }

        return GET_TOKEN(TK_NewLine);
    }

    else if(std::regex_match(std::string { this->source[index] }, std::regex("[0-9]"))) {
        std::string num;
        int startIndex = index;

        for(; index < this->source.length(); index++) {
            if(std::regex_match(std::string { this->source[index] }, std::regex("[0-9]"))) {
                num += std::string { this->source[index] };
            } else {
                index--;
                break;
            }
        }

        return Token(TK_Number, num, startIndex);
    }

    else if(MATCH_STR('\'')) {
        std::string tokenChar = "";
        int startIndex = index;

        if(this->source[index + 2] == '\'') {
            // 普通のchar値
            tokenChar = std::string { this->source[index + 1] };
            index += 2;
        } else if(this->source[index + 1] == '\\' && this->source[index + 3] == '\'') {
            // エスケープを含むchar値
            tokenChar = "\\" + std::string { this->source[index + 2] };
            index += 3;
        } else if(this->source[index + 1] == '\'') {
            // 文字の入っていない文字値
            Console::log(LogType_Error, 4139, { { "At", Token::getPositionText(g_cmd.argKeyAt("-i"), this->source, startIndex + 1) }, { "Expected", "{$LogDetailValue_ACharValue}" } });
            index += 1;
        } else {
            bool closed = false;
            int i;

            for(index++; index < this->source.length(); index++) {
                if(MATCH_STR('\'')) {
                    closed = true;
                    break;
                }
            }

            if(closed) {
                // 文字値が2文字以上の場合 (tooLongCharacterLengthエラー)
                Console::log(LogType_Error, 2471, { { "At", Token::getPositionText(g_cmd.argKeyAt("-i"), this->source, startIndex + 1) } });
            } else {
                //checkParenFinally();
                // 閉じクォーテーションがない場合 (EOFエラー)
                Console::log(LogType_Error, 1562, { { "At", Token::getPositionText(g_cmd.argKeyAt("-i"), this->source, startIndex + 2) }, { "Expected", "'''" } }, true);
            }

            return getNextToken();
        }

        return Token(TK_Char, std::string { tokenChar }, startIndex);
    }

    else if(MATCH_STR('\"')) {
        std::string str;
        int startIndex = index;

        for(index++; index < this->source.length(); index++) {
            if(this->source[index] != '\"') {
                str += this->source[index];
            } else {
                return Token(TK_String, str, startIndex);
            }
        }

        //checkParenFinally();
        Console::log(LogType_Error, 1562, { { "At", Token::getPositionText(g_cmd.argKeyAt("-i"), this->source, startIndex + 1) }, { "Expected", "'\"'" } }, true);
    }

    else {
        std::string tokenChars;
        int startIndex = index;

        while(index < this->source.length()) {
            // 正規表現では字句解析で判定済みの文字のみを使用してください (無限ループ防止)
            if(!std::regex_match(std::string { this->source[index] }, std::regex("[!?~+\\-*/%^=&|.,:;()\\[\\]<>{}'\" \n]"))) {
                tokenChars += this->source[index];
                index++;
            } else {
                index--;
                break;
            }
        }

        if(std::regex_match(tokenChars, std::regex("bol|break|byt|case|catch|chr|class|continue|default|dbl|elif|else|end|false|flt|for|if|import|int|lon|new|null|obj|package|private|public|return|sht|str|switch|true|ubyt|usht|uint|ulon|void"))) {
            return Token(TK_Keyword, tokenChars, startIndex);
        } else {
            return Token(TK_Identifier, tokenChars, startIndex);
        }
    }

    return GET_TOKEN(TK_Unknown);
}

void ches::cmd::Lexer::validateParen(Token token) {
    if(token.match(ByteSeq { TK_LeftParen, TK_LeftBracket, TK_LeftBrace })) {
        this->nestMap.at(token.type).first++;
        this->nestMap.at(token.type).second.push_back(token);
        this->openParens.push_back(token);
        return;
    }

    if(token.match(ByteSeq { TK_RightParen, TK_RightBracket, TK_RightBrace })) {
        // エラーの詳細用のマクロ
        #define DET_EXPECTED            "'" + latestOpenParen.getCloseParen().string + "'"
        #define DET_EXPECTED_POSITION   Token::getPositionText(g_cmd.argKeyAt("-i")
        #define DET_UNEXPECTED          "'" + token.string + "'"
        #define DET_UNEXPECTED_POSITION token.getPositionText(this->filePath, this->source)

        if(this->openParens.size() == 0) {
            Console::log(LogType_Error, 699, { { "At", DET_UNEXPECTED_POSITION }, { "Unexpected", DET_UNEXPECTED } });
            return;
        }

        Token paren = token.getOpenParen();
        Token latestOpenParen = this->openParens.back();

        if(latestOpenParen.type == TK_Unknown) {
            Console::log(LogType_Error, 699, { { "At", DET_UNEXPECTED_POSITION }, { "Unexpected", DET_UNEXPECTED } });
            return;
        }

        if(latestOpenParen.type != paren.type) {
            Console::log(LogType_Error, 698, { { "At", DET_EXPECTED_POSITION, this->source, latestOpenParen.index + 1) }, { "Expected", DET_EXPECTED } });
            this->nestMap.at(paren.type).first--;
            this->nestMap.at(latestOpenParen.type).first--;
            this->openParens.pop_back();
            return;
        }

        if(this->nestMap.at(paren.type).first < 0) {
            Console::log(LogType_Error, 699, { { "At", DET_UNEXPECTED_POSITION }, { "Unexpected", DET_UNEXPECTED } });
            return;
        }

        this->nestMap.at(paren.type).first--;
        this->openParens.pop_back();
    }
}
