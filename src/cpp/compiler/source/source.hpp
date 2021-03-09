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

void SourceFile::loadSourceFile(CPEG *cpeg) {
    try {
        this->source = FileManager::getText(this->filePath);
    } catch(FileManagerException excep) {
        throw excep;
    }

    try {
        SourceParser parser(cpeg, &this->source);

        this->syntaxTree = parser.toSyntaxTree();
        this->syntaxTree.print();
    } catch(CPEGException excep) {
        throw excep;
    }
}
