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

bool CPEGExpression::match(std::string *src, unsigned int &srcIndex) const {
    switch(this->type) {
        case CPEGExpression_CharClass: {
            try {
                if(src->size() + srcIndex < 1)
                    return false;

                std::regex pattern("[" + this->value + "]");

                if(!std::regex_match(src->substr(srcIndex, 1), pattern))
                    return false;

                srcIndex++;
            } catch(std::regex_error excep) {
                throw CPEGException(CPEGException_InvalidCPEGValue);
            }
        } return true;

        case CPEGExpression_String: {
            if(src->size() + srcIndex < this->value.size())
                return false;

            for(int i = 0; i < this->value.size(); i++)
                if(src->at(srcIndex + i) != this->value.at(i))
                    return false;

            srcIndex += this->value.size();
        } return true;

        case CPEGExpression_WildCard: {
            if(src->size() + srcIndex < 1)
                return false;

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
    SyntaxTree tree;

    std::cout << "- Source Parsing -" << std::endl;
    std::cout << std::endl;

    unsigned int srcIndex = 0;

    while(srcIndex < this->source->size()) {
        std::cout << srcIndex << " " << this->source->size() << std::endl;
        std::string matchedRuleName = this->searchMatchedRule(srcIndex);

        std::cout << "result [" << matchedRuleName << "]" << std::endl;
        std::cout << std::endl;

        if(matchedRuleName == "")
            throw CPEGException(CPEGException_NoCPEGRuleMatched);

        std::cout << "- - - - -" << std::endl;
        std::cout << std::endl;
    }

    return tree;
}

std::string SourceParser::searchMatchedRule(unsigned int &srcBegin) {
    for(CPEGRule rule : this->cpeg->rules) {
        std::cout << "checking [" << rule.name << "]" << std::endl;

        for(CPEGExpressionChoice choice : rule.exprChoices) {
            unsigned int choiceSrcBegin = srcBegin;
            bool choiceMatched = true;

            const CPEGExpressionSequenceGroup group = choice.exprSeqGroup;
            const std::vector<CPEGExpressionSequence> seqVec = group.exprSeqs;

            for(int seq_i = 0; seq_i < seqVec.size(); seq_i++) {
                unsigned int seqSrcBegin = choiceSrcBegin;
                bool seqMatched = true;

                const CPEGExpressionSequence seq = seqVec.at(seq_i);
                const std::vector<CPEGExpression> exprVec = seq.exprs;

                if(seq.props.lookbehindType != CPEGExpressionLookbehind_Default)
                    if(seq_i + 1 < seqVec.size())
                        throw CPEGException(CPEGException_LookbehindTargetNotExists);

                for(int expr_i = 0; expr_i < exprVec.size(); expr_i++) {
                    unsigned int exprSrcBegin = seqSrcBegin;
                    const CPEGExpression expr = exprVec.at(expr_i);

                    if(!expr.match(this->source, exprSrcBegin)) {
                        std::cout << "no(" << seqSrcBegin << "," << expr.value << ") ";
                        seqMatched = false;
                        break;
                    }

                    std::cout << "ok(" << seqSrcBegin << "," << expr.value << ") ";

                    seqSrcBegin = exprSrcBegin;
                }

                std::cout << std::endl;
                std::cout << std::endl;

                // note: Sequence が失敗したら次の Choice に移る

                if(!seqMatched) {
                    choiceMatched = false;
                    break;
                }

                choiceSrcBegin = seqSrcBegin;
            }

            // Choice が成功したら規則名を返す

            if(choiceMatched) {
                std::cout << std::endl;
                std::cout << "index change: " << srcBegin << " -> " << choiceSrcBegin << std::endl;
                srcBegin = choiceSrcBegin;
                return rule.name;
            }
        }
    }

    return "";
}
