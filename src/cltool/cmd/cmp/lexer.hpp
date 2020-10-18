#pragma once


namespace ches::cmd {
    typedef std::unordered_map<unsigned char, std::pair<int, ches::TokenSeq>>   nest_map;
    typedef std::pair<char, TokenType>  token_pair;


    class Lexer {
        std::string filePath;
        std::string source;

        int index = -1;
        nest_map nestMap = { { TK_LeftParen, {} }, { TK_LeftBracket, {} }, { TK_LeftBrace, {} } };
        std::deque<Token> openParens;

        vector_ext<token_pair> tokenPairs = {
            { '!', TK_Exclamation },
            { '?', TK_Question },
            { '~', TK_Tilde },
            { '+', TK_Plus },
            { '-', TK_Hyphen },
            { '*', TK_Asterisk },
            { '/', TK_Slash },
            { '%', TK_Percent },
            { '^', TK_Tilde },
            { '=', TK_Equal },
            { '|', TK_Pipe },
            { '&', TK_Ampersand },
            { '.', TK_Period },
            { ',', TK_Comma },
            { ':', TK_Colon },
            { ';', TK_Semicolon },
            { '<', TK_LeftAngleBracket },
            { '>', TK_RightAngleBracket }
        };

        vector_ext<token_pair> parenTokenPairs = {
            { '(', TK_LeftParen },
            { ')', TK_RightParen },
            { '[', TK_LeftBracket },
            { ']', TK_RightBracket },
            { '{', TK_LeftBrace },
            { '}', TK_RightBrace }
        };

    public:
        Lexer();

        Lexer(std::string filePath, std::string &source);

        TokenSeq splitTokens();

    private:
        void initTokenPairs();

        Token getNextToken();

        void validateParen(Token token);
    };
}
