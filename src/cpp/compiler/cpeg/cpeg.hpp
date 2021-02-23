/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../shared/filemanager/filemanager.hpp"

#include "../syntax/syntax.hpp"

#include "./cpeg.cpp"


using namespace ches::compiler;
using namespace ches::shared;


CPEGException::CPEGException() {}

CPEGException::CPEGException(CPEGExceptionType type) {
    this->type = type;
}

CPEGException::CPEGException(CPEGExceptionType type, std::unordered_map<std::string, std::string> detailMap) {
    this->type = type;
    this->detailMap = detailMap;
}


CPEGExpression::CPEGExpression() {}


CPEGExpressionSequence::CPEGExpressionSequence() {}


CPEGExpressionSequenceGroup::CPEGExpressionSequenceGroup() {}


CPEGExpressionChoice::CPEGExpressionChoice() {}


CPEGRule::CPEGRule() {}


CPEG::CPEG() {}

SyntaxTree getSyntaxTree(std::string &source) {
    SyntaxTree tree;

    return tree;
}

void CPEG::loadCPEGFile(std::string filePath) {
    std::vector<std::string> fileLines;

    try {
        fileLines = FileManager::getLines(filePath);
    } catch(FileManagerException excep) {
        throw excep;
    }

    for(std::string line : fileLines) {
        if(line.size() == 0)
            continue;

        if(line.at(0) == '#')
            continue;

        

        CPEGRule rule = CPEG::getCPEGRule(line);
        this->rules.push_back(rule);
    }
}
