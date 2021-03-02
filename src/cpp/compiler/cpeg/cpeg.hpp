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
#include <utility>
#include <vector>

#include "../../shared/filemanager/filemanager.hpp"

#include "../syntax/syntax.hpp"

#include "./cpeg.cpp"


using namespace ches::compiler;
using namespace ches::shared;


CPEGException::CPEGException() noexcept {}

CPEGException::CPEGException(CPEGExceptionType type) noexcept {
    this->type = type;
}

CPEGException::CPEGException(CPEGExceptionType type, std::unordered_map<std::string, std::string> detailMap) noexcept {
    this->type = type;
    this->detailMap = detailMap;
}


CPEGExpression::CPEGExpression() noexcept {}


CPEGExpressionSequence::CPEGExpressionSequence() noexcept {}


CPEGExpressionSequenceGroup::CPEGExpressionSequenceGroup() noexcept {}


CPEGExpressionChoice::CPEGExpressionChoice() noexcept {}


CPEGRule::CPEGRule() noexcept {}


CPEGTokensIndex::CPEGTokensIndex(std::vector<std::string> *tokens) {
    this->initMembers(tokens, 0, tokens->size());
}

CPEGTokensIndex::CPEGTokensIndex(std::vector<std::string> *tokens, unsigned int begin) {
    this->initMembers(tokens, begin, tokens->size() - begin);
}

CPEGTokensIndex::CPEGTokensIndex(std::vector<std::string> *tokens, unsigned int begin, unsigned int length) {
    this->initMembers(tokens, begin, length);
}

void CPEGTokensIndex::initMembers(std::vector<std::string> *tokens, unsigned int begin, unsigned int length) {
    this->_tokens = tokens;
    this->_begin = begin;
    this->_length = length;

    if(this->end() > tokens->size())
        throw CPEGException(CPEGException_InvalidCPEGTokensIndex);
}


std::regex CPEG::idTokenRegex = std::regex("[a-zA-Z0-9\\-_]+");
std::regex CPEG::symbolTokenRegex = std::regex("[:=.()\\[\\]>*+?&!]");
std::regex CPEG::spacingTokenRegex = std::regex("[ \t]");

CPEG::CPEG() noexcept {}

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

    CPEGRule tmpRule;

    for(std::string line : fileLines)
        if(CPEG::getCPEGRule(line, tmpRule))
            this->rules.push_back(tmpRule);
}
