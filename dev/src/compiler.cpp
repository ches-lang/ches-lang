#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "bytecode.cpp"
#include "command.cpp"
#include "console.cpp"
#include "filemanager.cpp"
#include "lexer.cpp"
#include "parser.cpp"



class Compiler {

public:

    std::string source;
    std::vector<Token> tokens;
    std::string filepath;
    bytecode bin;

    FileManager filemanager;
    Lexer lexer;
    Parser parser;
    Node node;
    Bytecode bc;



    Compiler(std::string path) {
        filepath = path;
    }

    void run() {
        source = filemanager.readText(filepath);
        lexer = Lexer(source);
        tokens = lexer.run();
        parser = Parser(tokens);
        node = parser.run();
        bc = Bytecode(node);
        bin = bc.run();
        filemanager.write(renamePathExt(filepath, "chesc"), bin);
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
