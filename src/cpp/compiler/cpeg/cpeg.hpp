/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
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


CPEGExpressionProperties::CPEGExpressionProperties() noexcept {}


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


CPEG::CPEG() noexcept {}

void CPEG::loadCPEGFile(std::string filePath) {
    std::vector<std::string> fileLines;

    try {
        fileLines = FileManager::getLines(filePath);
    } catch(FileManagerException excep) {
        throw excep;
    }

    CPEGRule tmpRule;

    for(std::string line : fileLines)
        if(CPEGParser::toCPEGRule(line, tmpRule))
            this->rules.push_back(tmpRule);
}

void CPEG::print() noexcept {
    std::cout << "- CPEG Rules -" << std::endl;
    std::cout << std::endl;

    for(CPEGRule rule : this->rules) {
        std::cout << "[" << rule.name << "]" << std::endl;

        for(CPEGExpressionChoice choice : rule.exprChoices) {
            std::cout << "  choice" << std::endl;

            for(CPEGExpressionSequence seq : choice.exprSeqGroup.exprSeqs) {
                std::string seqLoopTypeStr = CPEGExpressionProperties::toString(seq.props.loopType);
                std::string seqLookbehindTypeStr = CPEGExpressionProperties::toString(seq.props.lookbehindType);

                std::cout << "    seqs" << " " << seqLookbehindTypeStr << seqLoopTypeStr << std::endl;

                for(CPEGExpression expr : seq.exprs) {
                    CPEGExpressionType type = expr.type;
                    std::string typeName = CPEGExpression::toString(type);

                    std::string exprLoopTypeStr = CPEGExpressionProperties::toString(expr.props.loopType);
                    std::string exprLookbehindTypeStr = CPEGExpressionProperties::toString(expr.props.lookbehindType);

                    std::cout << "      " << typeName << ": " << expr.value << " " << exprLookbehindTypeStr << exprLoopTypeStr << std::endl;
                }
            }
        }

        std::cout << std::endl;
    }
}


std::regex CPEGParser::charClassTokenRegex = std::regex("\\[.*\\]");
std::regex CPEGParser::idTokenRegex = std::regex("[a-zA-Z0-9\\-_]+");
std::regex CPEGParser::spacingTokenRegex = std::regex("[ \t]");
std::regex CPEGParser::stringTokenRegex = std::regex("\".*\"");
std::regex CPEGParser::symbolTokenRegex = std::regex("[:=.()\\[\\]>*+?&!]");
