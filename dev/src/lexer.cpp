#pragma once

#include "lexer.hpp"



Options::Options() {}

Options::Options(std::unordered_map<std::string, std::string> opt) {
    map = opt;
}

bool Options::exists(std::string key) {
    return (map.find(key) != map.end());
}

std::string Options::get(std::string key) {
    return map[key];
}

void Options::set(std::string key, std::string value) {
    map[key] = value;
}

int Options::size() {
    return map.size();
}



Lexer::Lexer() {}

Lexer::Lexer(std::string srcpath, std::string src, Options opt) {
    sourcePath = srcpath;
    source = src;
    options = opt;
}

std::vector<Token> Lexer::getTokens() {
    std::vector<Token> tokens;
    Token tk;

    do {
        tk = scan();
        tokens.push_back(tk);
        //std::cout << (int)tk.type << "\t" << ((tk.string == "\n") ? "\\n" : tk.string) << std::endl;//
    } while(tk.type != TK_EndOfFile);

    checkParenFinally();

    if(Console::errored) {
        exit(-1);
    } else if(Console::warned) {
        if(!options.exists("-miss")) {
            exit(-1);
        }
    }

    return tokens;
}

Token Lexer::scan() {
    index++;
    char ch = source[index];

    if(index >= source.size())
        return Token(TK_EndOfFile, "", index);

    else if(ch == '!')
        return Token(TK_Exclamation, std::string{ch}, index);

    else if(ch == '?')
        return Token(TK_Question, std::string{ch}, index);

    else if(ch == '~')
        return Token(TK_Tilde, std::string{ch}, index);

    else if(ch == '+')
        return Token(TK_Plus, std::string{ch}, index);

    else if(ch == '-')
        return Token(TK_Hyphen, std::string{ch}, index);

    else if(ch == '*')
        return Token(TK_Asterisk, std::string{ch}, index);

    else if(ch == '/') {
        if(source[index + 1] == '*') {
            std::string res;
            int start = index;
            for(index += 2; index < source.length() - 1; index++) {
                if(source[index] == '*' && source[index + 1] == '/') {
                    index++;
                    return Token(TK_CommentOut, res, start);
                } else res += std::string{source[index]};
            }
            checkParenFinally();
            Console::log(LogType_Error, "1562", { { "At", Token::getPositionText(options.get("-i"), source, index) }, { "Expected", "'*/'" } }, true);
        } else if(source[index + 1] == '/') {
            std::string res;
            int start = index;
            for(index += 2; index < source.length(); index++) {
                if(source[index] == '\n') {
                    index--;
                    break;
                }
                res += std::string{source[index]};
            }
            return Token(TK_CommentOut, res, start);
        } else {
            return Token(TK_Slash, std::string{ch}, index);
        }
    }

    else if(ch == '%')
        return Token(TK_Percentage, std::string{ch}, index);

    else if(ch == '^')
        return Token(TK_Tilde, std::string{ch}, index);

    else if(ch == '=')
        return Token(TK_Equal, std::string{ch}, index);

    else if(ch == '|')
        return Token(TK_Pipe, std::string{ch}, index);

    else if(ch == '&')
        return Token(TK_Ampersand, std::string{ch}, index);

    else if(ch == '.')
        return Token(TK_Period, std::string{ch}, index);

    else if(ch == ',')
        return Token(TK_Comma, std::string{ch}, index);

    else if(ch == ':')
        return Token(TK_Colon, std::string{ch}, index);

    else if(ch == ';')
        return Token(TK_Semicolon, std::string{ch}, index);

    else if(ch == '(') {
        Token tk = Token(TK_LeftParen, std::string{ch}, index);
        checkParen(tk);
        return tk;
    }

    else if(ch == ')') {
        Token tk = Token(TK_RightParen, std::string{ch}, index);
        checkParen(tk);
        return tk;
    }

    else if(ch == '[') {
        Token tk = Token(TK_LeftBracket, std::string{ch}, index);
        checkParen(tk);
        return tk;
    }

    else if(ch == ']') {
        Token tk = Token(TK_RightBracket, std::string{ch}, index);
        checkParen(tk);
        return tk;
    }

    else if(ch == '<')
        return Token(TK_LeftAngleBracket, std::string{ch}, index);

    else if(ch == '>')
        return Token(TK_RightAngleBracket, std::string{ch}, index);

    else if(ch == '{') {
        Token tk = Token(TK_LeftBrace, std::string{ch}, index);
        checkParen(tk);
        return tk;
    }

    else if(ch == '}') {
        Token tk = Token(TK_RightBrace, std::string{ch}, index);
        checkParen(tk);
        return tk;
    }

    else if(ch == ' ') {
        for(int i = index - 1; i >= 0; i--)
            if(source[i] == ' ') continue;
            else if(source[i] == '\n') break;
            else return scan();

        if(source[index + 1] == ' ') {
            index += 1;
            return Token(TK_Indent, "  ", index - 1);
        } else {
            if(source[index + 1] == ' ') index++;
            Console::log(LogType_Error, "7903", { { "At", Token::getPositionText(options.get("-i"), source, index) } });
        }
    }

    else if(ch == '\n')
        if(source[index - 1] == '\n') return scan();
        else return Token(TK_NewLine, std::string{ch}, index);

    else if(std::regex_match(std::string{ch}, std::regex("[0-9]"))) {
        std::string num;
        std::string str;
        int start = index;
        for(; index < source.length(); index++) {
            str = std::string{source[index]};
            if(std::regex_match(str, std::regex("[0-9]")))
                num += str;
            else { index--; break; }
        }
        return Token(TK_Number, num, start);
    }

    else if(ch == '\'') {
        std::string chr = "";
        int start = index;
        if(source[index + 2] == '\'') {
            chr = std::string{source[index + 1]};
            index += 2;
        } else if(source[index + 1] == '\\' && source[index + 3] == '\'') {
            chr = "\\" + std::string{source[index + 2]};
            index += 3;
        } else if(source[index + 1] == '\'') {
            Console::log(LogType_Error, "4139", { { "At", Token::getPositionText(options.get("-i"), source, index) }, { "Expected", "a character" } });
            index += 1;
        } else {
            bool contain = false;
            int i;
            for(index += 1; index < source.length(); index++)
                if(source[index] == '\'') { contain = true; break; }
            if(contain) {
                Console::log(LogType_Error, "2471", { { "At", Token::getPositionText(options.get("-i"), source, index) } });
            } else {
                checkParenFinally();
                Console::log(LogType_Error, "1562", { { "At", Token::getPositionText(options.get("-i"), source, index) }, { "Expected", "'''" } }, true);
            }
            return scan();
        }
        return Token(TK_Character, std::string{chr}, start);
    }

    else if(ch == '\"') {
        std::string res;
        int start = index;
        for(index += 1; index < source.length(); index++) {
            if(source[index] != '\"') res += source[index];
            else return Token(TK_String, res, start);
        }
        checkParenFinally();
        Console::log(LogType_Error, "1562", { { "At", Token::getPositionText(options.get("-i"), source, index) }, { "Expected", "'\"'" } }, true);
    }

    else {
        std::string res;
        int start = index;
        while(index < source.length()) {
            // 正規表現では↑で判定済みの文字のみを使用してください。(無限ループ防止)
            if(!std::regex_match(std::string{source[index]}, std::regex("[!?~+\\-*/%^=&|.,:;()\\[\\]<>{}'\" \n]"))) {
                res += source[index];
                index++;
            }
            else { index--; break; }
        }
        if(std::regex_match(res, std::regex("bol|break|byt|default|case|catch|chr|class|continue|dbl|elif|else|false|flt|for|if|import|int|lon|new|null|obj|package|private|public|return|sht|str|switch|true|ubyt|usht|uint|ulon|void"))) {
            return Token(TK_Keyword, res, start);
        } else {
            return Token(TK_Identifier, res, start);
        }
    }

    return Token(TK_Unknown, std::string{ch});
}

void Lexer::checkParen(Token tk) {
    if(tk.match(std::vector<unsigned char> { TK_LeftParen, TK_LeftBracket, TK_LeftBrace })) {
        nest.at(tk.type).first++;
        nest.at(tk.type).second.push_back(tk);
        openParens.push_back(tk);
    } else if(tk.match(std::vector<unsigned char> { TK_RightParen, TK_RightBracket, TK_RightBrace })) {
        if(openParens.size() == 0) {
            Console::log(LogType_Error, "7904", { { "At", tk.getPositionText(sourcePath, source) }, { "Unexpected", "'" + tk.string + "'" } });
        } else {
            Token paren = tk.getOpenParen();
            Token latestOpenParen = openParens.back();

            if(latestOpenParen.type == TK_Unknown) {
                Console::log(LogType_Error, "7904", { { "At", tk.getPositionText(sourcePath, source) }, { "Unexpected", "'" + tk.string + "'" } });
            } else if(latestOpenParen.type != paren.type) {
                Console::log(LogType_Error, "7904", { { "At", latestOpenParen.getPositionText(sourcePath, source) }, { "Expected", "'" + latestOpenParen.getCloseParen().string + "'" } });
                nest.at(paren.type).first--;
                nest.at(latestOpenParen.type).first--;
                openParens.pop_back();
                openParens.pop_back();
            } else if(nest.at(paren.type).first < 0) {
                Console::log(LogType_Error, "7904", { { "At", tk.getPositionText(sourcePath, source) }, { "Unexpected", "'" + tk.string + "'" } });
            } else {
                nest.at(paren.type).first--;
                openParens.pop_back();
            }
        }
    }
}

void Lexer::checkParenFinally() {
    for(Token paren : openParens) {
        Console::log(LogType_Error, "7904", { { "At", paren.getPositionText(sourcePath, source) }, { "Expected", "'" + paren.getCloseParen().string + "'" } });
    }
}
