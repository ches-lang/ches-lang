/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


#include <string>
#include <vector>

#include "../source/source.hpp"

#include "./compiler.cpp"


using namespace ches::compiler;
using namespace ches::shared;


Compiler::Compiler(std::string sourcePath) {
    this->sourcePath = sourcePath;
}
