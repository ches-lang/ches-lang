/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
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


std::string cpegRelativePath = "/cpeg/syntax.cpeg";

Compiler::Compiler(std::string sourcePath) {
    this->sourcePath = sourcePath;

    try {
        this->sourceFiles = this->getSourceFiles();
    } catch(FileManagerException excep) {
        throw excep;
    }
}

void Compiler::compile(std::string outputFilePath) {
    if(this->sourceFiles.size() == 0)
        throw CompilerException(CompilerException_NoInputFile);
}

unsigned char* Compiler::getBytecode() {
    unsigned char *bytecode;

    return bytecode;
}

std::vector<SourceFile> Compiler::getSourceFiles() {
    std::vector<SourceFile> sourceFiles;

    std::vector<std::string> filePaths;
    std::vector<std::string> chesFilePaths;

    std::string homeDirPath = Configuration::getEnvironmentVariable(Configuration::homeDirEnvName);

    if(homeDirPath == "")
        throw ConfigurationException(ConfigurationException_InvalidEnvironmentVariable);

    CPEG cpeg;
    // todo: 後でパスを戻す
    // cpeg.loadCPEGFile(homeDirPath + Compiler::cpegRelativePath);
    cpeg.loadCPEGFile("/Users/Garnet3106/Desktop/Docs/Repos/chestnut/test/compiler/syntax.cpeg");

    if(FileManager::isDirectory(this->sourcePath)) {
        try {
            filePaths = FileManager::getAllFilePathsInDirectory(this->sourcePath);

            for(const std::string path : filePaths) {
                if(FileManager::matchExtensionName(path, "ches")) {
                    SourceFile file = SourceFile(path);
                    file.loadSourceFile(&cpeg);

                    sourceFiles.push_back(file);
                }
            }
        } catch(FileManagerException excep) {
            throw excep;
        }
    } else {
        sourceFiles = { this->sourcePath };
    }

    return sourceFiles;
}
