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

    std::map<std::string, std::string> options;
    std::string path;

    Compiler() {}

    Compiler(std::map<std::string, std::string> opt) {
        path = opt["-i"];std::cout<<path;
        options = opt;
    }

    void compile() {
        if(!FileManager::isDirectory(path)) {
            if(options.find("-o") != options.end()) {
                std::cout<<"notdir : "<<path<<std::endl;
            } else {
                FileManager::writeBytecode(FileManager::renamePathExt(path, "chesc"), getBytecode());
                std::cout<<"notdir : auto"<<std::endl;
            }
        } else {
            std::cout<<"dir : "<<path<<std::endl;
            std::vector<std::string> filepaths = FileManager::getFilePaths(path);
            std::vector<Bytecode> bytecodes;

            for(std::string path : filepaths) {
                if(FileManager::getPathExt(path).compare("ches") == 0) {
                    bytecodes.push_back(Compiler({ { "-i", path } }).getBytecode());
                }
            }
        }
    }

    Bytecode getBytecode() {
        std::string source = FileManager::readText(path);
        Lexer lexer(path, source);
        std::vector<Token> tokens = lexer.run();
        Parser parser(tokens);
        Node node = parser.run();
        return Bytecode(node);
    }
};
