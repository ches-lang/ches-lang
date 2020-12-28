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
#include <string>
#include <unordered_map>

#include "../datastruct/datastruct.hpp"

#include "./console.cpp"


ches::PropMap ches::Console::langPack;
std::string ches::Console::langPackFilePath = "./src/tools/cmd/chesc/data/langpack.pmap";

std::string ches::Console::langCode = "en";
int ches::Console::logLimit = -1;
