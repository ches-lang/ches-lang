/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
 *
 * Copyright © 2019-2021 Garnet3106 All rights reserved.
 */


#pragma once


#include <string>
#include <vector>

#include "../../shared/filemanager/filemanager.hpp"

#include "../source/source.hpp"

#include "./compiler.cpp"


using namespace ches::compiler;
using namespace ches::shared;


CompilerException::CompilerException() noexcept {}

CompilerException::CompilerException(CompilerExceptionType type) noexcept {
    this->type = type;
}

CompilerException::CompilerException(CompilerExceptionType type, std::unordered_map<std::string, std::string> detailMap) noexcept {
    this->type = type;
    this->detailMap = detailMap;
}


Compiler::Compiler(std::vector<std::string> srcFilePaths, std::string cpegPath) {
    try {
        this->cpeg.loadCPEGFile(cpegPath);
    } catch(CPEGException except) {
        throw except;
    } catch(FileManagerException except) {
        throw except;
    }

    for(std::string path : srcFilePaths)
        this->sourceFiles.push_back(SourceFile(path));
}

unsigned char* Compiler::compile() {
    if(this->sourceFiles.size() == 0)
        throw CompilerException(CompilerException_NoInputFile);

    for(SourceFile file : this->sourceFiles)
        file.loadSourceFile(&this->cpeg);

    return nullptr;
}

unsigned char* Compiler::getBytecode() {
    unsigned char *bytecode;

    return bytecode;
}
