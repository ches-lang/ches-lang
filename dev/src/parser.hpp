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

#define A(i) (tk.at(i >= 0 ? i : len - 1))
#define M(i, t, s) (Token(t, s).compare(A(i)))
#define IA(i) (A(i).index)
#define TA(i) (A(i).type)
#define TM(i, c) (TA(i) == c)
#define SA(i) (A(i).string)
#define SM(i, s) (SA(i) == s)
#define SMR(i, s) (std::regex_match(SA(i), std::regex(s)))



struct Line {
    std::vector<Token> tokens;
    int indent;
    int beginIndex;
    int endIndex;

    Line();

    Line(std::vector<Token> tokens, int beginIndex, int endIndex, int indent);
};



class Parser {

private:

    std::string sourcePath;
    std::string source;
    std::vector<Token> tokens;
    Options options;
    Node tree = Node(N_ROOT);
    std::vector<Line> lines;

    int lineIndex = 0;

public:

    Parser();

    Parser(std::string srcpath, std::string src, std::vector<Token> tk, Options opt);

    Node parse();

private:

    std::vector<Line> getLines();

    Node scanNextLine();

    Node getNode(std::vector<Token> tk, unsigned char defaultType = N_TOKEN);

    // 優先度高い             優先度低い
    // true → ope1 < ope2   false → ope1 >= ope2
    bool compareOpe(std::string ope1, std::string ope2);

    std::vector<Token> copy(int begin, int length, std::vector<Token> src);

    NextLine getLine(int startIndex);
};
