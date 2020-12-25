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


#include <string>
#include <vector>

#include "../../../lib/datastruct/datastruct.hpp"

#include "./compiler.cpp"


ches::Compiler::Compiler() {}

ches::Compiler::Compiler(std::string sourceCode) {
    this->sourceCode = sourceCode;
}

ches::byte* ches::Compiler::compile() {
    return 0;
}
