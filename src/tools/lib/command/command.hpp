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
    this->options = this->getCmdOptions();
}

void ches::Command::run() {
    std::cout << "name: " << this->name << std::endl;
    std::cout << "opts: ";

    for(auto [ key, value ] : this->options)
        std::cout << key << ":" << value << " ";

    std::cout << std::endl;
}

std::string ches::Command::getCmdName(std::string defaultCmdName) {
    if(args.size() == 0)
        return defaultCmdName;

    std::string firstArg = args.at(0);

    if(firstArg.size() >= 1 && firstArg.at(0) == '-')
        return defaultCmdName;

    return firstArg;
}

// spec: 基本的に無効なオプション引数は無視される
cmd_options ches::Command::getCmdOptions() {
    cmd_options result;

    for(int i = 0; i < this->args.size(); i++) {
        std::string targetArg = this->args.at(i);

        // note: オプション名が "-" のみの場合も弾く
        if(targetArg.size() <= 1 || targetArg.at(0) != '-')
            continue;

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
