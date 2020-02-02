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

#define A(i)        (tokens.at(i >= 0 ? i : len + 1))
#define M(i, t, s)  (Token(t, s).compare(A(i)))
#define IA(i)       (A(i).index)
#define LA(i)       (this->lines.at(i))
#define TA(i)       (A(i).type)
#define TM(i, c)    (TA(i) == c)
#define SA(i)       (A(i).string)
#define SM(i, s)    (SA(i) == s)
#define SMR(i, s)   (std::regex_match(SA(i), std::regex(s)))



struct Line {
    std::vector<Token> tokens;
    int nest;
    int beginIndex;
    int endIndex;

    Line();

    Line(std::vector<Token> tokens, int beginIndex, int endIndex, int nest);
};



struct ParenNest {
    int* paren, brack, angbrack, brace = 0;
    Token latestOpenParen;

    ParenNest();

    void checkParen(Token token);

    int* getNest(unsigned char type);
}



class Parser {

private:

    std::string sourcePath;
    std::string source;
    std::vector<Token> tokens;
    std::vector<Line> lines;
    Options options;
    Node tree = Node(N_ROOT);

    int lineIndex = -1;

public:

    Parser();

    Parser(std::string srcpath, std::string src, std::vector<Token> tk, Options opt);

    Node parse();

private:

    std::vector<Line> getLines();

    Node scanNextLine();

    Node scanNextNest(unsigned char nodeType = N_ROOT);

    Node getNode(Line line, unsigned char defaultType = N_ROOT);

    Node getNode(std::vector<Token> tokens, int nest = -1, unsigned char defaultType = N_TOKEN);

    std::vector<Token> removeParensAtBothEnd(std::vector<Token> tokens);

    // 優先度高い             優先度低い
    // true → ope1 < ope2   false → ope1 >= ope2
    bool compareOpe(std::string ope1, std::string ope2);

    std::vector<Token> copy(int begin, int length, std::vector<Token> src);

    NextLine getLine(int startIndex);
};
