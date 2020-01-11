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

    Options options;

    Compiler() {}

    Compiler(Options opt) {
        options = opt;
    }

    void compile() {
        if(options.exists("-o")) {
            if(FileManager::getFullPath(options.get("-i")) == FileManager::getFullPath(options.get("-o"))) {
                Console::warn("cwarn3405", "duplicate output file", { { "path", FileManager::getFullPath(options.get("-o")) } }, !options.exists("-miss"));
            }
        }

        if(!FileManager::isDirectory(options.get("-i"))) {
            if(options.exists("-o")) {
                FileManager::writeBytecode(options.get("-o"), getBytecode());
            } else {
                FileManager::writeBytecode(FileManager::renamePathExt(options.get("-i"), "chesc"), getBytecode());
            }
        } else {
            std::vector<std::string> filepaths = FileManager::getFilePaths(options.get("-i"));
            std::vector<Bytecode> bytecodes;

            for(int i = 0; i < filepaths.size(); i++) {
                if(FileManager::getPathExt(filepaths[i]) == "ches") {
                    bytecodes.push_back(Compiler(Options({ { "-i", filepaths[i] } })).getBytecode());
                } else {
                    filepaths.erase(filepaths.begin() + i);
                    i--;
                }
            }

            if(options.exists("-merge")) {
                std::string outpath;

                if(options.exists("-o")) {
                    outpath = options.get("-o");
                } else {
                    outpath = options.get("-i") + "/" + FileManager::getName(options.get("-i")) + ".chesc";
                }

                FileManager::writeBytecode(outpath, Bytecode(bytecodes));
            } else {
                for(int i = 0; i < bytecodes.size(); i++) {
                    FileManager::writeBytecode(FileManager::renamePathExt(filepaths[i], "chesc"), bytecodes[i]);
                }
            }
        }
    }

    Bytecode getBytecode() {
        std::string source = FileManager::readText(options.get("-i"));
        Lexer lexer(source, options);
        std::vector<Token> tokens = lexer.getTokens();
        Parser parser(options.get("-i"), source, tokens, options);
        Node node = parser.parse();
        return Bytecode(node);
    }
};
