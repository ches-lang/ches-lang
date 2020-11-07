#pragma once

#include "lexer.cpp"
#include "parser.cpp"

#include "compiler.hpp"


ches::cmd::Compiler::Compiler() {}

void ches::cmd::Compiler::checkOutputFilePath() {
    if(FileManager::isDirectory(g_cmd.argKeyAt("-i")))
        return;

    std::string inputFullPath = FileManager::getFullPath(this->inputFilePaths.at(0));
    std::string outputFullPath = FileManager::getFullPath(this->outputFilePath);

    // -miss が指定されている場合はツールを終了しない
    if(inputFullPath == outputFullPath)
        Console::log(LogType_Warning, 3405, { { "Path", FileManager::getFullPath(this->inputFilePaths.at(0)) } }, !g_cmd.existsArgKey("-miss"));
}

void ches::cmd::Compiler::compile() {
    // 入力/出力ファイルのパスを設定
    this->inputFilePaths = this->getInputFilePaths();

    if(g_cmd.existsArgKey("-o")) {
        this->outputFilePath = g_cmd.argKeyAt("-o");
        // 入力/出力ファイルが重複してないかチェックする
        this->checkOutputFilePath();
    } else if(!FileManager::isDirectory(g_cmd.argKeyAt("-i"))) {
        this->outputFilePath = FileManager::replacePathExt(this->inputFilePaths.at(0), "chesc");
    } else {
        Console::log(LogType_Error, 5900, {}, true);
    }

    ByteSeq bytecode = this->toBytecode();
    FileManager::writeBytecode(this->outputFilePath, bytecode);
}

std::vector<std::string> ches::cmd::Compiler::getInputFilePaths() {
    std::string inputPath = g_cmd.argKeyAt("-i");

    if(FileManager::isDirectory(inputPath)) {
        return FileManager::getFilePaths(inputPath);
    } else {
        return { inputPath };
    }
}

ches::ByteSeq ches::cmd::Compiler::toBytecode() {
    vector_ext<ByteSeq> bytecodes;

    std::string source;
    Lexer lexer;
    TokenSeq tokens;
    Parser parser;
    Node node;

    for(std::string path : this->inputFilePaths) {
        FileManager::readText(path, source);
        lexer = ches::cmd::Lexer(path, source);
        tokens = lexer.splitTokens();
        parser = Parser(path, source, tokens);
        node = parser.parse();
        bytecodes.push_back(ByteSeq(node, path, source));
    }

    return ByteSeq::join(bytecodes, ByteSeq((Byte)0x00, 5));
}
