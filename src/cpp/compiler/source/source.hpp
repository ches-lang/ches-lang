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


SourceFile::SourceFile(std::string filePath) noexcept {
    this->filePath = filePath;
}

void SourceFile::loadSourceFile() {
    try {
        this->source = FileManager::getText(this->filePath);
    } catch(FileManagerException excep) {
        throw excep;
    }
}

void SourceFile::loadSyntaxTree() {
    SyntaxTree tree;

    this->syntaxTree = tree;
}
