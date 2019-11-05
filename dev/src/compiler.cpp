#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "command.cpp"
#include "console.cpp"
#include "converter.cpp"
#include "filemanager.cpp"
#include "lexer.cpp"
#include "parser.cpp"



class Compiler {

public:

    std::string source;
    std::vector<std::pair<char, std::string>> tokens;

    std::string filepath;
    FileManager filemanager;
    Lexer lexer;
    Parser parser;
    Node node;
    Converter converter;
    std::vector<std::vector<std::vector<unsigned char>>> compiled;



    Compiler(std::string path) {
        filepath = path;
    }

    void run() {
        source = filemanager.readText(filepath);
        lexer = Lexer(source);
        tokens = lexer.run();
        parser = Parser(tokens);
        node = parser.run();
        converter = Converter(node, filepath);
        compiled = converter.run();
        filemanager.write(renamePathExt(filepath, "chesc"), compiled);
    }

    std::string renamePathExt(std::string path, std::string ext) {
        std::string res;
        std::string e;
        for(int i = path.length() - 1; i >= 0; i--)
            if(path[i] == '.') break;
            else e = path[i] + e;

        if(filepath == e) res = path + "." + ext;
        else res = path.substr(0, path.length() - e.length()) + ext;
        return res;
    }
};
