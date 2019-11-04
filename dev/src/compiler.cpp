#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "command.cpp"
#include "console.cpp"
#include "converter.cpp"
#include "lexer.cpp"
#include "parser.cpp"



class Compiler {

public:

    std::string source;
    std::vector<std::pair<char, std::string>> tokens;

    std::string filepath;
    Lexer lexer;
    Parser parser;
    Node node;
    Converter converter;
    std::vector<std::vector<unsigned char>> compiled;



    Compiler() {}

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
        parser = Parser(tokens);
        node = parser.run();
        converter = Converter(node, filepath);
        compiled = converter.run();
    }
};
