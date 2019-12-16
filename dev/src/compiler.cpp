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

    Compiler() {}

    Compiler(std::map<std::string, std::string> opt) {
        options = opt;
    }

    void compile() {
        if(options.find("-o") != options.end()) {
            if(FileManager::getFullPath(options["-i"]) == FileManager::getFullPath(options["-o"])) {
                Console::warn("cwarn3405", "duplicate output file", { { "path", FileManager::getFullPath(options["-o"]) } }, options.find("-miss") == options.end());
            }
        }

        if(!FileManager::isDirectory(options["-i"])) {
            if(options.find("-o") != options.end()) {
                FileManager::writeBytecode(options["-o"], getBytecode());
            } else {
                FileManager::writeBytecode(FileManager::renamePathExt(options["-i"], "chesc"), getBytecode());
            }
        } else {
            std::vector<std::string> filepaths = FileManager::getFilePaths(options["-i"]);
            std::vector<Bytecode> bytecodes;

            for(int i = 0; i < filepaths.size(); i++) {
                if(FileManager::getPathExt(filepaths[i]) == "ches") {
                    bytecodes.push_back(Compiler({ { "-i", filepaths[i] } }).getBytecode());
                } else {
                    filepaths.erase(filepaths.begin() + i);
                    i--;
                }
            }

            if(options.find("-merge") != options.end()) {
                std::string outpath;

                if(options.find("-o") != options.end()) {
                    outpath = options["-o"];
                } else {
                    outpath = options["-i"] + "/" + FileManager::getName(options["-i"]) + ".chesc";
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
        std::string source = FileManager::readText(options["-i"]);
        Lexer lexer(source, options);
        std::vector<Token> tokens = lexer.run();
        Parser parser(tokens);
        Node node = parser.run();
        return Bytecode(node);
    }
};
