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
#include <unordered_map>
#include <vector>

#include "../../shared/console/console.hpp"

#include "./syntax.cpp"


using namespace ches::compiler;
using namespace ches::shared;


CPEGExpressionException::CPEGExpressionException() {}

CPEGExpressionException::CPEGExpressionException(CPEGExpressionExceptionType type) {
    this->type = type;
}

CPEGExpressionException::CPEGExpressionException(CPEGExpressionExceptionType type, std::unordered_map<std::string, std::string> detailMap) {
    this->type = type;
    this->detailMap = detailMap;
}
