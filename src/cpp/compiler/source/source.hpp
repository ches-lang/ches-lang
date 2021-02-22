/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


#include <string>
#include <unordered_map>
#include <vector>

#include "../../shared/console/console.hpp"

#include "../syntax/syntax.hpp"

#include "./source.cpp"


using namespace ches::compiler;
using namespace ches::shared;


SourceFile::SourceFile(std::string filePath) {
    this->filePath = filePath;
}

void SourceFile::loadSourceFile() {
    try {
        this->source = this->getSource();
    } catch(FileManagerException excep) {
        throw excep;
    }

    try {
        this->syntaxTree = this->getSyntaxTree();
    } catch(CPEGExpressionException excep) {
        throw excep;
    }
}

std::string SourceFile::getSource() {
    if(this->filePath == "")
        throw FileManagerException(FileManagerException_InvalidPath);

    std::string source = "";

    return source;
}

SyntaxTree SourceFile::getSyntaxTree() {
    SyntaxTree tree;

    return tree;
}
