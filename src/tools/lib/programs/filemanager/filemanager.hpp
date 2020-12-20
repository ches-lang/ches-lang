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


#include <exception>
#include <filesystem>
#include <fstream>
#include <string>

#include "./filemanager.cpp"


ches::FileError::FileError() {}

ches::FileError::FileError(FileErrorType type) {
    this->type = type;
}
