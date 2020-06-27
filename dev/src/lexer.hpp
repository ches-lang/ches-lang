#pragma once



struct Options : std::unordered_map<std::string, std::string> {

    Options();

    Options(std::unordered_map<std::string, std::string> value);

    bool exists(std::string key);

    int size();
};



class Lexer {

private:

    std::string filePath;
    std::string source;
    Options options;

    int index = -1;
    std::unordered_map<unsigned char, std::pair<int, std::vector<Token>>> nest = { { TK_LeftParen, {} }, { TK_LeftBracket, {} }, { TK_LeftBrace, {} } };
    std::deque<Token> openParens;

public:

    Lexer();

    Lexer(std::string filePath, std::string source, Options options);

    std::vector<Token> splitTokens();

private:

    Token getNextToken();

    void validateParen(Token token);
};
