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

#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "./command.cpp"


ches::CommandError::CommandError() {}

ches::CommandError::CommandError(CommandErrorType type) {
    this->type = type;
}


ches::Command::Command() {}

ches::Command::Command(int argc, char* argv[], std::string defaultCmdName) {
    // note: コマンド名 (最初の引数) をスキップ
    for(int i = 1; i < argc; i++)
        this->args.push_back(std::string(argv[i]));

    this->name = this->getCmdName(defaultCmdName);
    this->optMap = this->getCmdOptions();
}

void ches::Command::run() {
    std::cout << "name: " << this->name << std::endl;
    std::cout << "opts: ";

    for(auto [ key, value ] : this->optMap)
        std::cout << key << ":" << value << " ";

    std::cout << std::endl;

    if(this->procMap.count(this->name) == 1) {
        this->procMap.at(this->name)(this->optMap);
    } else {
        throw CommandError(CommandError_UnknownSubCommand);
    }
}

std::string ches::Command::getCmdName(std::string defaultCmdName) {
    if(args.size() == 0) {
        this->usedDefaultName = true;
        return defaultCmdName;
    }

    std::string firstArg = args.at(0);

    if(firstArg.size() >= 1 && firstArg.at(0) == '-') {
        this->usedDefaultName = true;
        return defaultCmdName;
    }

    return firstArg;
}

// spec: 基本的に無効なオプション引数は無視される
// note: getCmdName()を呼び出した後に使用すること
cmd_opt_map ches::Command::getCmdOptions() {
    cmd_opt_map result;
    int beginIndex = this->usedDefaultName ? 0 : 1;

    for(int i = beginIndex; i < this->args.size(); i++) {
        std::string targetArg = this->args.at(i);

        // note: オプション名が "-" のみの場合も弾く
        if(targetArg.size() <= 1 || targetArg.at(0) != '-')
            throw CommandError(CommandError_InvalidArgument);

        std::string optName = targetArg.substr(1);
        std::string optArg = "";

        // note: ローカル変数を使っているのでexistsOptName()は使えない
        if(result.count(optName) == 1)
            throw CommandError(CommandError_DuplicatedOptionName);

        if(i + 1 < this->args.size()) {
            std::string nextArg = this->args.at(i + 1);

            if(nextArg.size() == 0 || nextArg.at(0) != '-') {
                optArg = nextArg;
                i++;
            }
        }

        result[optName] = optArg;
    }

    return result;
}
