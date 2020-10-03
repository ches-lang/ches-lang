#pragma once

#include "compiler.hpp"



Compiler::Compiler() {}

Compiler::Compiler(Command cmd) {
    this->cmd = cmd;
}

void Compiler::compile(std::string path) {
    this->path = path;

    if(this->cmd.existsKey("-o"))
        if(FileManager::getFullPath(path) == FileManager::getFullPath(this->cmd.at("-o")))
            Console::log(LogType_Warning, 3405, { { "Path", FileManager::getFullPath(this->cmd.at("-o")) } }, !this->cmd.existsKey("-miss"));

    ByteSeq byteSeq = this->getByteSeq();

    if(!FileManager::isDirectory(path)) {
        Console::exitIfDisplayed();

        if(this->cmd.existsKey("-o")) {
            FileManager::writeBytecode(this->cmd.at("-o"), byteSeq);
        } else {
            FileManager::writeBytecode(FileManager::replacePathExt(path, "chesc"), byteSeq);
        }
    } else {
        std::vector<std::string> dirFilePaths = FileManager::getFilePaths(path);
        LineSeq binSeq;

        for(int i = 0; i < dirFilePaths.size(); i++) {
            if(FileManager::getFileExt(dirFilePaths[i]) == "ches") {
                Compiler cmp;
                cmp.compile(dirFilePaths[i]);
                binSeq.push_back(cmp.getByteSeq());
            } else {
                dirFilePaths.erase(dirFilePaths.begin() + i);
                i--;
            }
        }

        if(this->cmd.existsKey("-merge")) {
            std::string outpath;

            if(this->cmd.existsKey("-o")) {
                outpath = this->cmd.at("-o");
            } else {
                outpath = path + "/" + FileManager::getFileName(path) + ".chesc";
            }

            //FileManager::writeBytecode(outpath, Bytecode(bytecodes));
        } else {
            for(int i = 0; i < binSeq.size(); i++)
                FileManager::writeBytecode(FileManager::replacePathExt(dirFilePaths[i], "chesc"), binSeq.at(i));
        }
    }

    if(!Console::errored && !Console::warned)
        Console::writeln("{$Message_CompilationSucceeded}");
}

ByteSeq Compiler::getByteSeq() {
    std::string source = FileManager::readText(this->path);
    Lexer lexer(this->path, source);
    std::vector<Token> tokens = lexer.splitTokens();
    Parser parser(this->path, source, tokens);
    Node node = parser.parse();
    return ByteSeq(node, this->path, source);
}
