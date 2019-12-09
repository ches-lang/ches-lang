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

class Instruction;

class Compiler {

public:

    std::string source;
    std::vector<Token> tokens;
    std::string filepath;
    Bytecode bytecode;

    Lexer lexer;
    Parser parser;
    Node node;



    Compiler() {}

    Compiler(std::string path) {
        filepath = path;
    }

    void run() {
        source = FileManager::readText(filepath);
        lexer = Lexer(filepath, source);
        tokens = lexer.run();
        parser = Parser(tokens);
        node = parser.run();
        bytecode = Bytecode(node);
        FileManager::writeBytecode(renamePathExt(filepath, "chesc"), bytecode);
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
