#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "command.cpp"
#include "console.cpp"
#include "lexer.cpp"
#include "parser.cpp"



extern std::string source;
extern std::vector<std::pair<char, std::string>> tokens;



class Compiler {

public:

    std::string filepath;
    Lexer lexer;
    Parser parser;

    Compiler() {
    }

    void compile() {
        lexer.analyze();
        parser.parse();
    }

    void loadFile(std::string path) {
        filepath = path;
        std::ifstream ifs(filepath);
        bool res = false;

        if(!ifs.is_open())
            error("file '" + filepath + "' is not exist", true);

        if(ifs.fail())
            error("loading file '" + filepath + "' is failed", true);

        std::string ln;
        while(getline(ifs, ln))
            source += ln + "\n";

        ifs.close();
    }
};
