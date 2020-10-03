#pragma once



class Lexer {

private:

    Command cmd;
    std::string filePath;
    std::string source;

    int index = -1;
    std::unordered_map<unsigned char, std::pair<int, std::vector<Token>>> nest = { { TK_LeftParen, {} }, { TK_LeftBracket, {} }, { TK_LeftBrace, {} } };
    std::deque<Token> openParens;

public:

    Lexer();

    Lexer(std::string filePath, std::string source, Command cmd);

    std::vector<Token> splitTokens();

private:

    Token getNextToken();

    void validateParen(Token token);
};
