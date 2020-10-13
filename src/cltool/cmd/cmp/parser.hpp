#pragma once

#define T_AT(idx)                   (tokens.at(idx >= 0 ? idx : tokens.size() + idx))
#define T_LINE_AT(idx)              (this->lines.at(idx))
#define T_MATCH(idx, typ, str)      (Token(typ, str).compare(T_AT(idx)))
#define T_TYPE_MATCH(idx, typ)      (T_AT(idx).type == typ)
#define T_TYPE_MATCH_REG(idx, typ)  (T_AT(idx).match(typ))
#define T_STR_MATCH(idx, str)       (T_AT(idx).string == str)
#define T_STR_MATCH_REG(idx, str)   (std::regex_match(T_AT(idx).string, std::regex(str)))

#define CURR_LINE                   (T_LINE_AT(this->lineIndex))


namespace ches::cmd {
    struct Line {
        std::vector<Token> tokens;
        int beginIndex;

        Line();

        Line(std::vector<Token> tokens);
    };


    struct ParenSeq {
        std::vector<int> parenNest = std::vector<int>(3, 0);
        std::vector<Token> parens;
        std::string sourcePath;
        std::string source;
        Token latestOpenParen;

        ParenSeq();

        ParenSeq(std::string sourcePath, std::string source);

        std::vector<Token> getOrderedParens(std::vector<Token> tokens);

    private:
        void addCloseParen(Token token);

        void addOpenParen(Token token);

        void checkParensFinally();

        std::vector<Token> removeSideParens(std::vector<Token> tokens);

        int getParenNumber(Byte type);
    };


    class Parser {
    private:
        std::string sourcePath;
        std::string source;
        std::vector<Token> tokens;
        std::vector<Line> lines;
        int blockNest = -1;
        Node tree = Node(ND_Root);

        int lineIndex = 0;

    public:
        Parser();

        Parser(std::string srcpath, std::string src, std::vector<Token> tk);

        Node parse();

    private:

        std::vector<Line> getLines();

        Node scanNextLine();

        Node scanNextNest(Byte nodeType = ND_Root);

        Node getNode(Line line, Byte defaultType = ND_Root);

        Node getNode(std::vector<Token> tokens, Byte defaultType = ND_Token);

        Node getLogicalExpressionNode(std::vector<Token> tokens);

        Node getCompareExpressionNode(std::vector<Token> tokens);

        Byte getOpeType(std::vector<Token> tokens, int index);

        // 優先度高い             優先度低い
        // true → ope1 < ope2   false → ope1 >= ope2
        bool compareOpe(std::string ope1, std::string ope2);

        std::vector<Token> copy(int begin, int length, std::vector<Token> src);

        Line getLine(int startIndex);
    };
}
