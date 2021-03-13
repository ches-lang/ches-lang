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

bool CPEGExpression::match(std::string *src, unsigned int &srcIndex, std::string &token) const {
    switch(this->type) {
        case CPEGExpression_CharClass: {
            try {
                if(src->size() + srcIndex < 1)
                    return false;

                std::regex pattern("[" + this->value + "]");

                if(!std::regex_match(src->substr(srcIndex, 1), pattern))
                    return false;

                token = { src->at(srcIndex) };

                srcIndex++;
            } catch(std::regex_error excep) {
                throw CPEGException(CPEGException_InvalidCPEGValue);
            }
        } return true;

        case CPEGExpression_String: {
            if(src->size() < this->value.size() + srcIndex)
                return false;

            for(int i = 0; i < this->value.size(); i++)
                if(src->at(srcIndex + i) != this->value.at(i))
                    return false;

            token = this->value;

            srcIndex += this->value.size();
        } return true;

        case CPEGExpression_WildCard: {
            if(src->size() + srcIndex < 1)
                return false;

            token = { src->at(srcIndex) };

            srcIndex++;
        } return true;

        default:
        throw CPEGException(CPEGException_UnknownCPEGExpressionType);
    }

    throw CPEGException(CPEGException_UnknownCPEGExpressionType);
}


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


SourceParser::SourceParser(CPEG *cpeg, std::string *source) {
    this->cpeg = cpeg;
    this->source = source;
}

SyntaxTree SourceParser::toSyntaxTree() {
    std::cout << "- Source Parsing -" << std::endl;
    std::cout << std::endl;

    SyntaxTree tree;

    SyntaxTreeNode rootNode;
    rootNode.name = "root";
    rootNode.nodes = this->toSyntaxTreeNode();

    tree.root = rootNode;

    return tree;
}

std::vector<SyntaxTreeNode> SourceParser::toSyntaxTreeNode() {
    std::vector<SyntaxTreeNode> nodes;

    while(this->index < this->source->size()) {
        bool succeeded = false;

        for(int i = 0; i < this->cpeg->rules.size(); i++) {
            std::vector<std::string> tokens;

            if(this->ruleSuccessful(this->cpeg->rules.at(i), tokens)) {
                std::string name = this->cpeg->rules.at(i).name;

                std::cout << "rule [" << name << "]" << std::endl;
                std::cout << std::endl;

                SyntaxTreeNode newNode;
                newNode.name = name;
                newNode.tokens = tokens;
                nodes.push_back(newNode);

                succeeded = true;
            }
        }

        if(!succeeded) {
            std::cout << "err index: " << this->index << std::endl;
            throw CPEGException(CPEGException_NoSucceededCPEGRule);
        }

        std::cout << "- - - - -" << std::endl;
        std::cout << std::endl;
    }

    return nodes;
}

bool SourceParser::ruleSuccessful(CPEGRule &rule, std::vector<std::string> &tokens) {
    std::cout << "checking [" << rule.name << "]" << std::endl;

    bool choiceSucceeded = false;

    for(CPEGExpressionChoice choice : rule.exprChoices) {
        unsigned int tmpChoiceIndex = this->index;

        const CPEGExpressionSequenceGroup group = choice.exprSeqGroup;
        const std::vector<CPEGExpressionSequence> seqVec = group.exprSeqs;

        std::vector<std::string> tmpChoiceTokens;

        for(int seq_i = 0; seq_i < seqVec.size(); seq_i++) {
            unsigned int tmpSeqIndex = tmpChoiceIndex;
            bool escapeSeqProc = false;

            const CPEGExpressionSequence seq = seqVec.at(seq_i);
            const std::vector<CPEGExpression> exprVec = seq.exprs;

            std::vector<std::string> tmpSeqTokens;

            if(seq.props.lookbehindType != CPEGExpressionLookbehind_Default)
                if(seq_i + 1 < seqVec.size())
                    throw CPEGException(CPEGException_LookbehindTargetNotExists);

            for(int expr_i = 0; expr_i < exprVec.size(); expr_i++) {
                unsigned int tmpExprIndex = tmpSeqIndex;

                const CPEGExpression expr = exprVec.at(expr_i);

                std::string newToken;

                if(expr.match(this->source, tmpExprIndex, newToken)) {
                    std::cout << "ok(" << tmpSeqIndex << "," << expr.value << ") ";
                    tmpSeqTokens.push_back(newToken);
                    tmpSeqIndex = tmpExprIndex;
                } else {
                    std::cout << "no(" << tmpSeqIndex << "," << expr.value << ") ";
                    escapeSeqProc = true;
                    break;
                }
            }

            std::cout << std::endl;
            std::cout << std::endl;

            // note: Sequences が失敗したら次の Choice に移る

            if(escapeSeqProc)
                break;

            for(const std::string tk : tmpSeqTokens)
                tmpChoiceTokens.push_back(tk);

            tmpChoiceIndex = tmpSeqIndex;

            choiceSucceeded = true;
        }

        // Choice が成功したら true を返す

        if(choiceSucceeded) {
            std::cout << std::endl;
            std::cout << "changed index: " << this->index << " -> " << tmpChoiceIndex << std::endl;

            this->index = tmpChoiceIndex;

            tokens = tmpChoiceTokens;

            return true;
        }
    }

    return false;
}
