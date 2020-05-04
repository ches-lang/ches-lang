#pragma once

#include <deque>
#include <iostream>
#include <regex>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include "console.cpp"
#include "lexer.cpp"
#include "syntax.cpp"

#define A(i)        (tokens.at(i >= 0 ? i : tokens.size() + i))
#define M(i, t, s)  (Token(t, s).compare(A(i)))
#define IA(i)       (A(i).index)
#define LA(i)       (this->lines.at(i))
#define CURR_LINE   (LA(this->lineIndex))
#define TA(i)       (A(i).type)
#define TM(i, c)    (TA(i) == c)
#define TMR(i, c)   (A(i).match(c))
#define SA(i)       (A(i).string)
#define SM(i, s)    (SA(i) == s)
#define SMR(i, s)   (std::regex_match(SA(i), std::regex(s)))



struct Line {
    std::vector<Token> tokens;
    int nest;
    int beginIndex;

    Line();

    Line(std::vector<Token> tokens);
};



struct ParenSeq {
    std::vector<int> nestOfParens = std::vector<int>(3, 0);
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

    void checkCloseParensFinally();

    std::vector<Token> removeSurroundingParens(std::vector<Token> tokens);

    int getNestIndex(Byte type);
};



class Parser {

private:

    std::string sourcePath;
    std::string source;
    std::vector<Token> tokens;
    std::vector<Line> lines;
    Options options;
    Node tree = Node(ND_Root);

    int lineIndex = 0;

public:

    Parser();

    Parser(std::string srcpath, std::string src, std::vector<Token> tk, Options opt);

    Node parse();

private:

    std::vector<Line> getLines();

    Node scanNextLine();

    Node scanNextNest(Byte nodeType = ND_Root);

    Node getNode(Line line, Byte defaultType = ND_Root);

    Node getNode(std::vector<Token> tokens, int nest = -1, Byte defaultType = ND_Token);

    Node getLogicalExpressionNode(std::vector<Token> tokens);

    Node getCompareExpressionNode(std::vector<Token> tokens);

    Byte getOpeType(std::vector<Token> tokens, int index);

    // 優先度高い             優先度低い
    // true → ope1 < ope2   false → ope1 >= ope2
    bool compareOpe(std::string ope1, std::string ope2);

    std::vector<Token> copy(int begin, int length, std::vector<Token> src);

    Line getLine(int startIndex);
};
