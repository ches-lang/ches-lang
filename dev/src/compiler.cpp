#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "command.cpp"
#include "console.cpp"
#include "lexer.cpp"



extern std::string source;
extern std::vector<std::pair<char, char>> tokens;



class Compiler {

public:

    std::string filepath;
    Lexer lexer;

    Compiler(std::string path) {
        filepath = path;
        loadFile();
    }

    void compile() {
        lexer.analyze();
    }

private:

    void loadFile() {
        std::ifstream ifs(filepath);

        if(!ifs.is_open()) {
            error("file '" + filepath + "' is not exist");
        }

        else if(ifs.fail()) {
            error("loading file '" + filepath + "' is failed");
        }

        else {
            std::string ln;
            while(getline(ifs, ln))
                source += ln + "\n";
        }

        ifs.close();
    }
};
