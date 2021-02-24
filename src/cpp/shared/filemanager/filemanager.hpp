/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "./filemanager.cpp"


using namespace ches::shared;


FileManagerException::FileManagerException() noexcept {}

FileManagerException::FileManagerException(FileManagerExceptionType type) noexcept {
    this->type = type;
}

FileManagerException::FileManagerException(FileManagerExceptionType type, std::string target) noexcept {
    this->type = type;
    this->target = target;
}
