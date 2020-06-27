#pragma once

#include "compiler.hpp"



Compiler::Compiler() {}

Compiler::Compiler(Options opt) {
    options = opt;
}

void Compiler::compile() {
    if(options.exists("-o"))
        if(FileManager::getFullPath(options["-i"]) == FileManager::getFullPath(options["-o"]))
            Console::log(LogType_Warning, "3405", { { "Path", FileManager::getFullPath(options["-o"]) } }, !options.exists("-miss"));

    ByteSeq byteSeq = this->getByteSeq();

    if(!FileManager::isDirectory(options["-i"])) {
        Console::exitIfDisplayed();

        if(options.exists("-o")) {
            FileManager::writeBytecode(options["-o"], byteSeq);
        } else {
            FileManager::writeBytecode(FileManager::renamePathExt(options["-i"], "chesc"), byteSeq);
        }
    } else {
        std::vector<std::string> filepaths = FileManager::getFilePaths(options["-i"]);
        LineSeq binSeq;

        for(int i = 0; i < filepaths.size(); i++) {
            if(FileManager::getFileExt(filepaths[i]) == "ches") {
                binSeq.push_back(Compiler(Options({ { "-i", filepaths[i] } })).getByteSeq());
            } else {
                filepaths.erase(filepaths.begin() + i);
                i--;
            }
        }

        if(options.exists("-merge")) {
            std::string outpath;

            if(options.exists("-o")) {
                outpath = options["-o"];
            } else {
                outpath = options["-i"] + "/" + FileManager::getFileName(options["-i"]) + ".chesc";
            }

            //FileManager::writeBytecode(outpath, Bytecode(bytecodes));
        } else {
            for(int i = 0; i < binSeq.size(); i++)
                FileManager::writeBytecode(FileManager::renamePathExt(filepaths[i], "chesc"), binSeq.at(i));
        }
    }

    if(!Console::errored && !Console::warned)
        Console::writeln("{$Message_CompilationSucceeded}");
}

ByteSeq Compiler::getByteSeq() {
    std::string filePath = options["-i"];
    std::string source = FileManager::readText(filePath);
    Lexer lexer(filePath, source, options);
    std::vector<Token> tokens = lexer.splitTokens();
    Parser parser(filePath, source, tokens, options);
    Node node = parser.parse();
    return ByteSeq(node, filePath, source);
}
