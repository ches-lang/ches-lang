#pragma once


namespace ches::cmd {
    class Lexer {
        std::string filePath;
        std::string source;

        int index = -1;
        nest_map nestMap = { { TK_LeftParen, {} }, { TK_LeftBracket, {} }, { TK_LeftBrace, {} } };
        std::deque<Token> openParens;

    public:
        Lexer();

        Lexer(std::string filePath, std::string &source);

        std::vector<Token> splitTokens();

    private:
        void initTokenPairs();

        Token getNextToken();

        void validateParen(Token token);
    };
}
