#pragma once

#include "compiler.hpp"



Compiler::Compiler() {}

void Compiler::compile(std::string path) {
    this->path = path;

    if(g_cmd_data.exists("-o"))
        if(FileManager::getFullPath(path) == FileManager::getFullPath(g_cmd_data["-o"]))
            Console::log(LogType_Warning, "3405", { { "Path", FileManager::getFullPath(g_cmd_data["-o"]) } }, !g_cmd_data.exists("-miss"));

    ByteSeq byteSeq = this->getByteSeq();

    if(!FileManager::isDirectory(path)) {
        Console::exitIfDisplayed();

        if(g_cmd_data.exists("-o")) {
            FileManager::writeBytecode(g_cmd_data["-o"], byteSeq);
        } else {
            FileManager::writeBytecode(FileManager::renamePathExt(path, "chesc"), byteSeq);
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

        if(g_cmd_data.exists("-merge")) {
            std::string outpath;

            if(g_cmd_data.exists("-o")) {
                outpath = g_cmd_data["-o"];
            } else {
                outpath = path + "/" + FileManager::getFileName(path) + ".chesc";
            }

            //FileManager::writeBytecode(outpath, Bytecode(bytecodes));
        } else {
            for(int i = 0; i < binSeq.size(); i++)
                FileManager::writeBytecode(FileManager::renamePathExt(dirFilePaths[i], "chesc"), binSeq.at(i));
        }
    }

    if(!Console::errored && !Console::warned)
        Console::writeln("{$Message_CompilationSucceeded}");
}

ByteSeq Compiler::getByteSeq() {
    std::string filePath = this->path;
    std::string source = FileManager::readText(filePath);
    Lexer lexer(filePath, source);
    std::vector<Token> tokens = lexer.splitTokens();
    Parser parser(filePath, source, tokens);
    Node node = parser.parse();
    return ByteSeq(node, filePath, source);
}
