#pragma once

#include "cmddata.hpp"



CommandData::CommandData() {}

CommandData::CommandData(std::string cmdName, std::unordered_map<std::string, std::string> cmdArgs) {
    this->cmdName = cmdName;

    for(auto [ key, val ] : cmdArgs)
        (*this)[key] = val;
}

bool CommandData::exists(std::string key) {
    return this->find(key) != this->end();
}

CommandData g_cmd_data;
