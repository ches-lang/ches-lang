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
    } while(tk.type != ENDOFFILE);

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
        return Token(ENDOFFILE, "", index);

    else if(ch == '!')
        return Token(EXCLAMATION, std::string{ch}, index);

    else if(ch == '?')
        return Token(QUESTION, std::string{ch}, index);

    else if(ch == '~')
        return Token(TILDE, std::string{ch}, index);

    else if(ch == '+')
        return Token(PLUS, std::string{ch}, index);

    else if(ch == '-')
        return Token(HYPHEN, std::string{ch}, index);

    else if(ch == '*')
        return Token(ASTERISK, std::string{ch}, index);

    else if(ch == '/') {
        if(source[index + 1] == '*') {
            std::string res;
            int start = index;
            for(index += 2; index < source.length() - 1; index++) {
                if(source[index] == '*' && source[index + 1] == '/') {
                    index++;
                    return Token(COMMENTOUT, res, start);
                } else res += std::string{source[index]};
            }
            checkParenFinally();
            Console::error("cerr1562", "expected EOF", { { "at", Token::getPositionText(options.get("-i"), source, index) }, { "expected", "'*/'" } }, true);
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
            return Token(COMMENTOUT, res, start);
        } else {
            return Token(SLASH, std::string{ch}, index);
        }
    }

    else if(ch == '%')
        return Token(PERCENTAGE, std::string{ch}, index);

    else if(ch == '^')
        return Token(TILDE, std::string{ch}, index);

    else if(ch == '=')
        return Token(EQUAL, std::string{ch}, index);

    else if(ch == '|')
        return Token(PIPE, std::string{ch}, index);

    else if(ch == '&')
        return Token(AMPERSAND, std::string{ch}, index);

    else if(ch == '.')
        return Token(PERIOD, std::string{ch}, index);

    else if(ch == ',')
        return Token(COMMA, std::string{ch}, index);

    else if(ch == ':')
        return Token(COLON, std::string{ch}, index);

    else if(ch == ';')
        return Token(SEMICOLON, std::string{ch}, index);

    else if(ch == '(') {
        Token tk = Token(LPAREN, std::string{ch}, index);
        checkParen(tk);
        return tk;
    }

    else if(ch == ')') {
        Token tk = Token(RPAREN, std::string{ch}, index);
        checkParen(tk);
        return tk;
    }

    else if(ch == '[') {
        Token tk = Token(LBRACK, std::string{ch}, index);
        checkParen(tk);
        return tk;
    }

    else if(ch == ']') {
        Token tk = Token(RBRACK, std::string{ch}, index);
        checkParen(tk);
        return tk;
    }

    else if(ch == '<')
        return Token(LANGBRACK, std::string{ch}, index);

    else if(ch == '>')
        return Token(RANGBRACK, std::string{ch}, index);

    else if(ch == '{') {
        Token tk = Token(LBRACE, std::string{ch}, index);
        checkParen(tk);
        return tk;
    }

    else if(ch == '}') {
        Token tk = Token(RBRACE, std::string{ch}, index);
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
            return Token(INDENT, "  ", index - 1);
        } else {
            if(source[index + 1] == ' ') index++;
            Console::error("cerr7903", "invalid indent space", { { "at", Token::getPositionText(options.get("-i"), source, index) } }, false);
        }
    }

    else if(ch == '\n')
        if(source[index - 1] == '\n') return scan();
        else return Token(NEWLINE, std::string{ch}, index);

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
        return Token(NUMBER, num, start);
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
            Console::error("cerr4139", "too short character length", { { "at", Token::getPositionText(options.get("-i"), source, index) }, { "expected", "a character" } }, false);
            index += 1;
        } else {
            bool contain = false;
            int i;
            for(index += 1; index < source.length(); index++)
                if(source[index] == '\'') { contain = true; break; }
            if(contain) {
                Console::error("cerr2471", "too long character length", { { "at", Token::getPositionText(options.get("-i"), source, index) } }, false);
            } else {
                checkParenFinally();
                Console::error("cerr1562", "expected EOF", { { "at", Token::getPositionText(options.get("-i"), source, index) }, { "expected", "'''" } }, true);
            }
            return scan();
        }
        return Token(CHARACTER, std::string{chr}, start);
    }

    else if(ch == '\"') {
        std::string res;
        int start = index;
        for(index += 1; index < source.length(); index++) {
            if(source[index] != '\"') res += source[index];
            else return Token(STRING, res, start);
        }
        checkParenFinally();
        Console::error("cerr1562", "expected EOF", { { "at", Token::getPositionText(options.get("-i"), source, index) }, { "expected", "'\"'" } }, true);
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
        if(std::regex_match(res, std::regex("bol|break|byt|default|case|catch|chr|class|continue|dbl|elif|else|false|flt|for|import|int|lon|new|null|obj|package|private|public|return|sht|str|switch|true|ubyt|usht|uint|ulon|void"))) {
            return Token(KEYWORD, res, start);
        } else {
            return Token(IDENTIFIER, res, start);
        }
    }

    return Token(UNKNOWN, std::string{ch});
}

void Lexer::checkParen(Token tk) {
    if(tk.match(std::vector<unsigned char> { LPAREN, LBRACK, LBRACE })) {
        nest.at(tk.type).first++;
        nest.at(tk.type).second.push_back(tk);
        openParens.push_back(tk);
    } else if(tk.match(std::vector<unsigned char> { RPAREN, RBRACK, RBRACE })) {
        if(openParens.size() == 0) {
            Console::error("cerr7904", "unexpected closing parenthesis or bracket", { { "at", tk.getPositionText(sourcePath, source) }, { "unexpected", "'" + tk.string + "'" } }, false);
        } else {
            Token paren = tk.getOpenParen();
            Token latestOpenParen = openParens.back();

            if(latestOpenParen.type == UNKNOWN) {
                Console::error("cerr7904", "unexpected closing parenthesis or bracket", { { "at", tk.getPositionText(sourcePath, source) }, { "unexpected", "'" + tk.string + "'" } }, false);
            } else if(latestOpenParen.type != paren.type) {
                Console::error("cerr7904", "expected closing parenthesis or bracket", { { "at", latestOpenParen.getPositionText(sourcePath, source) }, { "expected", "'" + latestOpenParen.getCloseParen().string + "'" } }, false);
                nest.at(paren.type).first--;
                nest.at(latestOpenParen.type).first--;
                openParens.pop_back();
                openParens.pop_back();
            } else if(nest.at(paren.type).first < 0) {
                Console::error("cerr7904", "unexpected closing parenthesis or bracket", { { "at", tk.getPositionText(sourcePath, source) }, { "unexpected", "'" + tk.string + "'" } }, false);
            } else {
                nest.at(paren.type).first--;
                openParens.pop_back();
            }
        }
    }
}

void Lexer::checkParenFinally() {
    for(Token paren : openParens) {
        Console::error("cerr7904", "expected closing parenthesis or bracket", { { "at", paren.getPositionText(sourcePath, source) }, { "expected", "'" + paren.getCloseParen().string + "'" } }, false);
    }
}
