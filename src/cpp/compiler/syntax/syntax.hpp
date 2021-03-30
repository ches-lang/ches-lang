/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
 *
 * Copyright © 2019-2021 Garnet3106 All rights reserved.
 */


#pragma once


#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "../../shared/filemanager/filemanager.hpp"

#include "./syntax.cpp"


using namespace ches::compiler;
using namespace ches::shared;


SyntaxTreeNode::SyntaxTreeNode() noexcept {}

void SyntaxTreeNode::print(int nestCount) {
    std::string indent(nestCount * 4, ' ');

    std::cout << indent << "\033[36m[" << this->name << "]\033[m";

    for(std::string tk : this->tokens) {
        tk = std::regex_replace(tk, std::regex("\\\\"), "\\\\");
        tk = std::regex_replace(tk, std::regex(" "), "\\s");
        tk = std::regex_replace(tk, std::regex("　"), "\\S");
        tk = std::regex_replace(tk, std::regex("\t"), "\\t");
        tk = std::regex_replace(tk, std::regex("\n"), "\\n");

        std::cout << " \033[44m" << tk << "\033[m";
    }

    std::cout << std::endl;

    for(SyntaxTreeNode node : this->nodes)
        node.print(nestCount + 1);
}


SyntaxTree::SyntaxTree() noexcept {}

void SyntaxTree::print() {
    std::cout << "- Syntax Tree -" << std::endl;
    std::cout << std::endl;

    this->root.print();

    std::cout << std::endl;
}
