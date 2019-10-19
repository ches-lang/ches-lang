#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "command.cpp"
#include "compiler.cpp"
#include "console.cpp"

#define UNKNOWN 0
#define LTR_NUM 1
#define LTR_CHR 2
#define LTR_STR 3
#define OPE_ADD 4
#define OPE_SUB 5
#define OPE_MUL 6
#define OPE_DIV 7
#define PRN_LFT 8
#define PRN_RGT 9



std::string source;
std::vector<std::pair<char, char>> tokens;



class Lexer {

public:

    Lexer() {

    }

    void analyze() {
        std::pair<char, char> res;
        do {
            res = scan();
            tokens.push_back(res);
            std::cout << res.second;
        } while(res.first != -1);
    }

private:

    int index = 0;

    std::pair<char, char> scan() {
        char token = '\0';
        char type = -1;

        token = source[index];

        if(index >= source.length())
            return std::make_pair(type, token);

        //else if()
            

        else
            type = UNKNOWN;

        index++;
        return std::make_pair(type, token);
    }
};
