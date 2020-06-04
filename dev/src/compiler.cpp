#pragma once

#include "compiler.hpp"



Compiler::Compiler() {}

Compiler::Compiler(Options opt) {
    options = opt;
}

void Compiler::compile() {
    if(options.exists("-o"))
        if(FileManager::getFullPath(options.get("-i")) == FileManager::getFullPath(options.get("-o")))
            Console::log(LogType_Warning, "3405", { { "Path", FileManager::getFullPath(options.get("-o")) } }, !options.exists("-miss"));

    Bytecode bytecode = getBytecode();

    if(!FileManager::isDirectory(options.get("-i"))) {
        Console::exitIfDisplayed();

        if(options.exists("-o")) {
            FileManager::writeBytecode(options.get("-o"), bytecode);
        } else {
            FileManager::writeBytecode(FileManager::renamePathExt(options.get("-i"), "chesc"), bytecode);
        }
    } else {
        std::vector<std::string> filepaths = FileManager::getFilePaths(options.get("-i"));
        std::vector<Bytecode> bytecodes;

        for(int i = 0; i < filepaths.size(); i++) {
            if(FileManager::getFileExt(filepaths[i]) == "ches") {
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
                outpath = options.get("-i") + "/" + FileManager::getFileName(options.get("-i")) + ".chesc";
            }

            FileManager::writeBytecode(outpath, Bytecode(bytecodes));
        } else {
            for(int i = 0; i < bytecodes.size(); i++) {
                FileManager::writeBytecode(FileManager::renamePathExt(filepaths[i], "chesc"), bytecodes[i]);
            }
        }
    }

    if(!Console::errored && !Console::warned)
        Console::writeln("{$Message_CompilationSucceeded}");
}

Bytecode Compiler::getBytecode() {
    std::string source = FileManager::readText(options.get("-i"));
    Lexer lexer(options.get("-i"), source, options);
    std::vector<Token> tokens = lexer.splitTokens();
    Parser parser(options.get("-i"), source, tokens, options);
    Node node = parser.parse();
    return Bytecode(node);
}
