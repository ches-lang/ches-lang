#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "command.cpp"
#include "console.cpp"
#include "lexer.cpp"



class Compiler {

public:

    std::string source;
    std::vector<std::pair<char, std::string>> tokens;

    Compiler() {}

    std::string filepath;
    Lexer lexer;

    void load(std::string path) {
        filepath = path;
        std::ifstream ifs(path);
        if(!ifs.is_open())
            error("specified file '" + path + "' was not found", true);
        if(ifs.fail())
            error("file read error", true);

        source = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

        ifs.close();
    }

    void run() {
        lexer = Lexer(source);
        tokens = lexer.run();
    }
};
