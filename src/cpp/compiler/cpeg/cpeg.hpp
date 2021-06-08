/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
 *
 * Copyright © 2019-2021 Garnet3106 All rights reserved.
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


CPEGExpressionRenaming::CPEGExpressionRenaming() noexcept {}


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
        throw CPEGException(CPEGException_UnknownExpressionType);
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
        throw CPEGException(CPEGException_InvalidTokensIndex);
}


CPEG::CPEG() noexcept {}

void CPEG::loadCPEGFile(std::string filePath) {
    std::vector<std::string> fileLines;

    try {
        fileLines = FileManager::getLines(filePath);
    } catch(FileManagerException except) {
        throw except;
    }

    CPEGRule tmpRule;

    int line_i = 0;

    try {
        for(; line_i < fileLines.size(); line_i++)
            if(CPEGParser::toCPEGRule(fileLines.at(line_i), tmpRule))
                this->rules.push_back(tmpRule);
    } catch(CPEGException except) {
        throw except;
    } catch(std::out_of_range except) {
        std::cout << "outofrange " << line_i << std::endl;
    }
}

void CPEG::print() noexcept {
    std::cout << "- CPEG Rules -" << std::endl;
    std::cout << std::endl;

    for(CPEGRule rule : this->rules) {
        std::cout << "[" << rule.name << "]" << std::endl;

        for(CPEGExpressionChoice choice : rule.exprChoices) {
            std::cout << "  choice" << std::endl;

            for(CPEGExpressionSequence seq : choice.exprSeqGroup.exprSeqs) {
                std::string seqRenaming = seq.renaming.toString();

                std::string seqLoopTypeStr = CPEGExpressionProperties::toString(seq.props.loopType);
                std::string seqLookaheadTypeStr = CPEGExpressionProperties::toString(seq.props.lookaheadType);

                std::cout << "    seqs" << " " << seqLookaheadTypeStr << seqLoopTypeStr << " " << seqRenaming << std::endl;

                for(CPEGExpression expr : seq.exprs) {
                    CPEGExpressionType type = expr.type;
                    std::string typeName = CPEGExpression::toString(type);

                    std::string exprRenaming = expr.renaming.toString();

                    std::string exprLoopTypeStr = CPEGExpressionProperties::toString(expr.props.loopType);
                    std::string exprLookaheadTypeStr = CPEGExpressionProperties::toString(expr.props.lookaheadType);

                    std::cout << "      " << typeName << ": " << expr.value << " " << exprLookaheadTypeStr << exprLoopTypeStr << " " << exprRenaming << std::endl;
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
std::regex CPEGParser::symbolTokenRegex = std::regex("[:|=.()\\[\\]>*+?&!]");


SourceParser::SourceParser(CPEG *cpeg, std::string *source) {
    this->cpeg = cpeg;
    this->source = source;
}

bool SourceParser::sequenceSuccessful(const unsigned int nest, unsigned int &index, CPEGExpressionSequence &seq, SyntaxTreeNode &node) {
    std::string consoleIndent(nest * 4, ' ');
    std::string consoleIndentPlus((nest + 1) * 4, ' ');

    std::cout << std::endl;
    std::cout << consoleIndentPlus << "sequence check (" << index << ")" << std::endl;

    unsigned int tmpIndex = index;
    SyntaxTreeNode tmpNode = node;

    int loopCount = 0;
    std::pair<unsigned int, int> minAndMaxCount = seq.props.getMinAndMaxCount();

    std::cout << consoleIndentPlus << "sequence min/max count: " << minAndMaxCount.first << " " << minAndMaxCount.second << std::endl;
    std::cout << std::endl;

    while(index <= this->source->size()) {
        unsigned int tmpSeqIndex = tmpIndex;
        SyntaxTreeNode tmpSeqNode = tmpNode;

        bool seqSucceeded = false;

        if(index != this->source->size()) {
            bool eachSeqSucceeded = true;

            for(int expr_i = 0; expr_i < seq.exprs.size(); expr_i++) {
                if(!this->expressionSuccessful(nest + 1, tmpSeqIndex, seq.exprs.at(expr_i), tmpSeqNode)) {
                    std::cout << consoleIndentPlus << "no (" << tmpSeqIndex << "," << seq.exprs.at(expr_i).value << ")" << std::endl;
                    eachSeqSucceeded = false;
                    break;
                } else {
                    std::cout << consoleIndentPlus << "ok (" << tmpSeqIndex << "," << seq.exprs.at(expr_i).value << ")" << std::endl;
                }
            }

            if(eachSeqSucceeded) {
                std::cout << consoleIndentPlus << "tmp sequence succeeded (" << tmpSeqIndex << ")" << std::endl;

                tmpIndex = tmpSeqIndex;
                tmpNode = tmpSeqNode;

                loopCount++;
                seqSucceeded = true;
            } else {
                std::cout << consoleIndentPlus << "tmp sequence failed (" << tmpSeqIndex << ")" << std::endl;
            }
        }

        if(loopCount >= minAndMaxCount.first) {
            if(!seqSucceeded && minAndMaxCount.second == -1
                    || loopCount <= minAndMaxCount.second) {
                // std::cout << seq.renaming.getAvailable() << " " << seq.renaming.getNewName() << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
                SourceParser::addSequenceNodeToNode(node, tmpNode, seq.renaming);
                // node = tmpNode;

                index = tmpIndex;

                return true;
            }
        }

        if(!seqSucceeded)
            return false;
    }

    return false;
}

bool SourceParser::expressionSuccessful(const unsigned int nest, unsigned int &index, CPEGExpression &expr, SyntaxTreeNode &node) {
    int loopCount = 0;
    std::pair<unsigned int, int> minAndMaxCount = expr.props.getMinAndMaxCount();

    while(index <= this->source->size()) {
        bool succeeded = false;

        if(index != this->source->size() && this->expressionTokenSuccessful(nest, index, expr, node)) {
            succeeded = true;
            loopCount++;
        }

        if(loopCount >= minAndMaxCount.first) {
            if(!succeeded && minAndMaxCount.second == -1
                    || loopCount <= minAndMaxCount.second)
                return true;
        }

        if(!succeeded)
            return false;
    }

    return false;
}

bool SourceParser::expressionTokenSuccessful(const unsigned int nest, unsigned int &index, CPEGExpression &expr, SyntaxTreeNode &node) {
    bool isLookaheadExpr = expr.props.lookaheadType != CPEGExpressionLookahead_Default;
    bool isLookaheadNegative = expr.props.lookaheadType == CPEGExpressionLookahead_Negative;

    switch(expr.type) {
        case CPEGExpression_CharClass: {
            try {
                if(this->source->size() + index < 1)
                    return isLookaheadNegative;

                std::regex pattern("[" + expr.value + "]");

                if(!std::regex_match(this->source->substr(index, 1), pattern))
                    return isLookaheadNegative;

                if(!isLookaheadExpr) {
                    SourceParser::addExpressionTokenToNode(node, { this->source->at(index) }, expr.renaming);
                    index++;
                }
            } catch(std::regex_error excep) {
                throw CPEGException(CPEGException_InvalidValue);
            }
        } return !isLookaheadNegative;

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
                throw CPEGException(CPEGException_InvalidRuleName);

            unsigned int tmpIndex = index;
            SyntaxTreeNode newNode;

            if(!this->ruleSuccessful(nest, tmpIndex, newRule, newNode))
                return isLookaheadNegative;

            if(!isLookaheadExpr) {
                SourceParser::addExpressionNodeToNode(node, newNode, expr.renaming);
                std::cout << index << " " << tmpIndex << std::endl;
                index = tmpIndex;
            }
        } return !isLookaheadNegative;

        case CPEGExpression_String: {
            if(this->source->size() < index + expr.value.size())
                return isLookaheadNegative;

            for(int i = 0; i < expr.value.size(); i++)
                if(this->source->at(index + i) != expr.value.at(i))
                    return isLookaheadNegative;

            if(!isLookaheadExpr) {
                SourceParser::addExpressionTokenToNode(node, expr.value, expr.renaming);
                index += expr.value.size();
            }
        } return !isLookaheadNegative;

        case CPEGExpression_WildCard: {
            if(this->source->size() + index < 1)
                return isLookaheadNegative;

            if(!isLookaheadExpr) {
                SourceParser::addExpressionTokenToNode(node, { this->source->at(index) }, expr.renaming);
                index++;
            }
        } return !isLookaheadNegative;

        default:
        throw CPEGException(CPEGException_UnknownExpressionType);
    }

    throw CPEGException(CPEGException_UnknownExpressionType);
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
            if(this->cpeg->rules.at(i).isMacro)
                continue;

            SyntaxTreeNode newNode;

            if(this->ruleSuccessful(0, this->index, this->cpeg->rules.at(i), newNode)) {
                std::string name = this->cpeg->rules.at(i).name;

                std::cout << "rule [" << name << "]" << std::endl;
                std::cout << std::endl;

                nodes.push_back(newNode);

                succeeded = true;

                break;
            }
        }

        if(!succeeded) {
            std::cout << "err index: " << this->index << std::endl;
            throw CPEGException(CPEGException_NoSucceededRule, { { "index", std::to_string(this->index) } });
        }

        std::cout << "- - - - -" << std::endl;
        std::cout << std::endl;
    }

    return nodes;
}

bool SourceParser::ruleSuccessful(const unsigned int nest, unsigned int &index, CPEGRule &rule, SyntaxTreeNode &node) {
    std::string consoleIndent(nest * 4, ' ');
    std::string consoleIndentPlus((nest + 1) * 4, ' ');

    std::cout << std::endl << consoleIndent << "checking [" << rule.name << "]" << std::endl;

    if(index >= this->source->size()) {
        std::cout << consoleIndent << "index out of range (" << index << ")" << std::endl;
        throw CPEGException(CPEGException_IndexOutOfRange);
    }

    for(CPEGExpressionChoice choice : rule.exprChoices) {
        bool choiceSucceeded = true;

        unsigned int tmpChoiceIndex = index;
        SyntaxTreeNode tmpChoiceNode;

        const CPEGExpressionSequenceGroup group = choice.exprSeqGroup;
        const std::vector<CPEGExpressionSequence> groupSeqs = group.exprSeqs;

        for(int seq_i = 0; seq_i < groupSeqs.size(); seq_i++) {
            // unsigned int tmpSeqIndex = tmpChoiceIndex;
            // SyntaxTreeNode tmpSeqNode;

            CPEGExpressionSequence seq = groupSeqs.at(seq_i);

            if(seq.props.lookaheadType != CPEGExpressionLookahead_Default)
                if(seq_i + 1 < groupSeqs.size())
                    throw CPEGException(CPEGException_LookaheadTargetNotExists);

            // note: Sequences が失敗したら次の Choice に移る

            if(!this->sequenceSuccessful(nest, tmpChoiceIndex, seq, tmpChoiceNode)) {
                std::cout << consoleIndentPlus << "sequence failed (" << tmpChoiceIndex << ")" << std::endl;
                choiceSucceeded = false;
                break;
            } else {
                std::cout << consoleIndentPlus << "sequence succeeded (" << tmpChoiceIndex << ")" << std::endl;
            }

            // tmpChoiceIndex = tmpSeqIndex;
            // tmpChoiceNode = tmpSeqNode;
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
