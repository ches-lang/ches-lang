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
#include <vector>

#include "../../shared/filemanager/filemanager.hpp"

#include "./syntax.cpp"


using namespace ches::compiler;
using namespace ches::shared;


SyntaxTreeNode::SyntaxTreeNode() noexcept {}

void SyntaxTreeNode::print(int nestCount) {
    std::string indent(nestCount * 4, ' ');

    std::cout << indent << "[" << this->name << "]" << std::endl;

    for(std::string token : this->tokens)
        std::cout << indent << "  " << token << std::endl;

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
