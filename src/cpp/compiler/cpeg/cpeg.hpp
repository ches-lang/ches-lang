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

std::pair<unsigned int, int> CPEGExpressionProperties::getMinAndMaxCount() const {
    switch(this->loopType) {
        case CPEGExpressionLoop_Default:
        return { 1, 1 };

        case CPEGExpressionLoop_ZeroOrMore:
        return { 0, -1 };

        case CPEGExpressionLoop_OneOrMore:
        return { 1, -1 };

        case CPEGExpressionLoop_ZeroOrOne:
        return { 0, 1 };

        default:
        throw CPEGException(CPEGException_UnknownCPEGExpressionType);
    }
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

bool SourceParser::expressionMatch(unsigned int nest, unsigned int &index, CPEGExpression &expr, SyntaxTreeNode &node) {
    int loopCount = 0;
    std::pair<unsigned int, int> minAndMaxCount = expr.props.getMinAndMaxCount();

    while(index <= this->source->size()) {
        bool matched = false;

        if(index != this->source->size() && this->expressionTokenMatch(nest, index, expr, node)) {
            matched = true;
            loopCount++;
        }

        if(loopCount >= minAndMaxCount.first) {
            if(!matched && minAndMaxCount.second == -1)
                return true;

            if(loopCount <= minAndMaxCount.second)
                return true;
        }

        if(!matched)
            return false;
    }

    return false;
}

bool SourceParser::expressionTokenMatch(unsigned int nest, unsigned int &index, CPEGExpression &expr, SyntaxTreeNode &node) {
    switch(expr.type) {
        case CPEGExpression_CharClass: {
            try {
                if(this->source->size() + index < 1)
                    return false;

                std::regex pattern("[" + expr.value + "]");

                if(!std::regex_match(this->source->substr(index, 1), pattern))
                    return false;

                node.tokens.push_back({ this->source->at(index) });

                index++;
            } catch(std::regex_error excep) {
                throw CPEGException(CPEGException_InvalidCPEGValue);
            }
        } return true;

        case CPEGExpression_ID: {
            CPEGRule newRule;
            bool newRuleExists = false;

            for(int i = 0; i < this->cpeg->rules.size(); i++) {
                if(this->cpeg->rules.at(i).name == expr.value) {
                    newRule = this->cpeg->rules.at(i);
                    newRuleExists = true;

                    break;
                }
            }

            if(!newRuleExists)
                throw CPEGException(CPEGException_InvalidCPEGRuleName);

            SyntaxTreeNode newNode;

            if(!this->ruleSuccessful(nest, index, newRule, newNode))
                return false;

            node.nodes.push_back(newNode);
        } return true;

        case CPEGExpression_String: {
            if(this->source->size() < index + expr.value.size())
                return false;

            for(int i = 0; i < expr.value.size(); i++)
                if(this->source->at(index + i) != expr.value.at(i))
                    return false;

            node.tokens.push_back(expr.value);

            index += expr.value.size();
        } return true;

        case CPEGExpression_WildCard: {
            if(this->source->size() + index < 1)
                return false;

            node.tokens.push_back({ this->source->at(index) });

            index++;
        } return true;

        default:
        throw CPEGException(CPEGException_UnknownCPEGExpressionType);
    }

    throw CPEGException(CPEGException_UnknownCPEGExpressionType);
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
            SyntaxTreeNode newNode;

            if(this->ruleSuccessful(0, this->index, this->cpeg->rules.at(i), newNode)) {
                std::string name = this->cpeg->rules.at(i).name;

                std::cout << "tokens: " << newNode.tokens.size() << std::endl;

                std::cout << "rule [" << name << "]" << std::endl;
                std::cout << std::endl;

                nodes.push_back(newNode);

                succeeded = true;

                break;
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

bool SourceParser::ruleSuccessful(unsigned int nest, unsigned int &index, CPEGRule &rule, SyntaxTreeNode &node) {
    std::string consoleIndent(nest * 4, ' ');
    std::string consoleIndentPlus((nest + 1) * 4, ' ');

    std::cout << std::endl << consoleIndent << "checking [" << rule.name << "]" << std::endl;

    if(index >= this->source->size()) {
        std::cout << consoleIndent << "index out of range (" << index << ")" << std::endl;
        throw CPEGException(CPEGException_IndexOutOfRange);
    }

    bool choiceSucceeded = false;

    for(CPEGExpressionChoice choice : rule.exprChoices) {
        unsigned int tmpChoiceIndex = index;

        const CPEGExpressionSequenceGroup group = choice.exprSeqGroup;
        const std::vector<CPEGExpressionSequence> seqVec = group.exprSeqs;

        SyntaxTreeNode tmpChoiceNode;

        for(int seq_i = 0; seq_i < seqVec.size(); seq_i++) {
            unsigned int tmpSeqIndex = tmpChoiceIndex;
            bool escapeSeqProc = false;

            const CPEGExpressionSequence seq = seqVec.at(seq_i);
            const std::vector<CPEGExpression> exprVec = seq.exprs;

            SyntaxTreeNode tmpSeqNode;

            if(seq.props.lookbehindType != CPEGExpressionLookbehind_Default)
                if(seq_i + 1 < seqVec.size())
                    throw CPEGException(CPEGException_LookbehindTargetNotExists);

            for(int expr_i = 0; expr_i < exprVec.size(); expr_i++) {
                unsigned int tmpExprIndex = tmpSeqIndex;

                CPEGExpression expr = exprVec.at(expr_i);

                if(this->expressionMatch(nest + 1, tmpExprIndex, expr, tmpSeqNode)) {
                    std::cout << consoleIndentPlus << "ok (" << tmpSeqIndex << "," << expr.value << ")" << std::endl;
                    tmpSeqIndex = tmpExprIndex;
                } else {
                    std::cout << consoleIndentPlus << "no (" << tmpSeqIndex << "," << expr.value << ")" << std::endl;
                    escapeSeqProc = true;
                    break;
                }
            }

            // note: Sequences が失敗したら次の Choice に移る

            if(escapeSeqProc)
                break;

            tmpChoiceIndex = tmpSeqIndex;
            tmpChoiceNode = tmpSeqNode;

            choiceSucceeded = true;
        }

        // Choice が成功したら true を返す

        if(choiceSucceeded) {
            std::cout << std::endl;
            std::cout << consoleIndent << "changed index: " << index << " -> " << tmpChoiceIndex << std::endl;

            index = tmpChoiceIndex;

            node = tmpChoiceNode;
            node.name = rule.name;

            return true;
        }
    }

    return false;
}
