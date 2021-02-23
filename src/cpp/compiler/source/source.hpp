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

#include "../cpeg/cpeg.hpp"
#include "../syntax/syntax.hpp"

#include "./source.cpp"


using namespace ches::compiler;
using namespace ches::shared;


SourceFile::SourceFile(std::string filePath) {
    this->filePath = filePath;
    this->loadSourceFile();
}

void SourceFile::loadSourceFile() {
    try {
        this->source = this->getSource();
    } catch(FileManagerException excep) {
        throw excep;
    }

    try {
        this->syntaxTree = this->getSyntaxTree();
    } catch(CPEGException excep) {
        throw excep;
    }
}

std::string SourceFile::getSource() {
    std::string source = "";

    try {
        source = FileManager::getText(this->filePath);
    } catch(FileManagerException excep) {
        throw excep;
    }

    return source;
}

SyntaxTree SourceFile::getSyntaxTree() {
    SyntaxTree tree;

    return tree;
}
