/* 
 * 
 * Chestnut 0.0.0
 * 
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 * 
 * Copyright © 2020 Garnet3106 All rights reserved. 
 * 
 */


#pragma once


#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../filemanager/filemanager.hpp"

#include "./syntax.cpp"


ches::SyntaxNotationError::SyntaxNotationError() {}

ches::SyntaxNotationError::SyntaxNotationError(SyntaxNotationErrorType type) {
    this->type = type;
}


ches::SyntaxProp::SyntaxProp() {}

ches::SyntaxProp::SyntaxProp(std::string name) {
    this->name = name;
}

ches::SyntaxProp::SyntaxProp(std::string name, syntax_item_seq items) {
    this->name = name;
    this->items = items;
}


ches::Syntax::Syntax() {}

void ches::Syntax::load(std::string filePath) {
    std::vector<std::string> fileContent = FileManager::readTextLines(filePath);
    syntax_token_lines tokenLines = Syntax::toTokenLines(fileContent);
    this->loadProps(tokenLines);

    // for(std::vector<std::string> tokenSeq : tokenLines) {
    //     for(std::string token : tokenSeq) {
    //         std::cout << token << "|";
    //     }
    // }

    // std::cout << std::endl;

    for(auto [ labelName, propSeq ] : this->syntaxProps) {
        std::cout << "label: " << labelName << std::endl << std::endl;

        for(SyntaxProp prop : propSeq) {
            std::cout << prop.name << " = ";

            for(syntax_item item : prop.items) {
                for(syntax_token token : item) {
                    std::cout << token.second << "(" << token.first << ") ";
                }

                std::cout << ", ";
            }

            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}

void ches::Syntax::loadProps(syntax_token_lines tokenLines) {
    std::string labelName = "";

    for(std::vector<std::string> line : tokenLines) {
        if(line.size() == 0)
            continue;

        if(line.at(0) == "#") {
            if(line.size() != 2)
                throw SyntaxNotationError(SyntaxNotation_InvalidSyntax);

            labelName = line.at(1);
            continue;
        }

        if(line.size() >= 3 && line.at(1) == "=") {
            SyntaxProp prop(line.at(0));
            syntax_item item;

            for(int i = 2; i < line.size(); i++) {
                std::string token = line.at(i);
                bool isLastToken = i + 1 == line.size();

                if(token == "," || isLastToken) {
                    if(isLastToken)
                        item.push_back(Syntax::getSyntaxToken(token));

                    std::cout<<":"<<(item.size() == 0 ? "..." : item.at(0).second)<<std::endl;
                    prop.items.push_back(item);
                    item.clear();
                    continue;
                }

                item.push_back(Syntax::getSyntaxToken(token));
            }

            if(this->syntaxProps.count(labelName) == 0)
                throw SyntaxNotationError(SyntaxNotation_UnknownLabelName);

            this->syntaxProps[labelName].push_back(prop);
            continue;
        }

        throw SyntaxNotationError(SyntaxNotation_InvalidSyntax);
    }
}
