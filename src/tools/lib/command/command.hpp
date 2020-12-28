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
#include <regex>
#include <unordered_map>
#include <vector>

#include "../datastruct/datastruct.hpp"

#include "./command.cpp"


ches::CommandError::CommandError() {}

ches::CommandError::CommandError(CommandErrorType type) {
    this->type = type;
}

ches::CommandError::CommandError(CommandErrorType type, log_detail_map details) {
    this->type = type;
    this->details = details;
}


std::string ches::CommandOptionKeys::logLimit = "log-limit";

std::string ches::SettingKeys::lang = "lang";
std::string ches::SettingKeys::logLimit = "logLimit";


std::string ches::Command::settingFilePath = "./src/tools/cmd/chesc/data/settings.pmap";

ches::Command::Command() {}

// todo: それぞれの処理を関数ごとに分ける
// excep: CommandError
ches::Command::Command(int argc, char* argv[], std::string defaultCmdName) {
    try {
        // note: コマンド名 (最初の引数) をスキップ
        for(int i = 1; i < argc; i++)
            this->args.push_back(std::string(argv[i]));

        this->name = this->getCmdName(defaultCmdName);
        this->options = this->getCmdOptions();

        // proc: 設定ファイルのロード

        this->settings.load(Command::settingFilePath);

        // proc: 言語コードの変更

        if(this->settings.exists(SettingKeys::lang))
            Console::langCode = this->settings.get(SettingKeys::lang);

        // proc: -log-limitオプションのロード

        this->proceedLogLimitOption();
    } catch(CommandError excep) {
        throw excep;
    }
}

void ches::Command::run() {
    if(this->procMap.count(this->name) == 0)
        throw CommandError(CommandError_UnknownSubCommand);

    this->procMap.at(this->name)(this->options, settings);
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
ches::PropMap ches::Command::getCmdOptions() {
    PropMap options;
    int beginIndex = this->usedDefaultName ? 0 : 1;

    for(int i = beginIndex; i < this->args.size(); i++) {
        std::string targetArg = this->args.at(i);

        // note: オプション名が "-" のみの場合も弾く
        if(targetArg.size() <= 1 || targetArg.at(0) != '-')
            throw CommandError(CommandError_InvalidOptionName);

        std::string optName = targetArg.substr(1);
        std::string optArg = "";

        // note: ローカル変数を使っているのでexistsOptName()は使えない
        if(options.count(optName) == 1)
            throw CommandError(CommandError_DuplicatedOptionName);

        if(i + 1 < this->args.size()) {
            std::string nextArg = this->args.at(i + 1);

            if(nextArg.size() == 0 || nextArg.at(0) != '-') {
                optArg = nextArg;
                i++;
            }
        }

        options[optName] = optArg;
    }

    return options;
}

// excep: CommandError
void ches::Command::proceedLogLimitOption() {
    std::string logLimit = CommandOptionKeys::logLimit;
    std::string value = this->options.at(logLimit);
    std::regex regex("[^0-9]");

    CommandError error = CommandError(CommandError_InvalidOptionValue, { { "key", logLimit }, { "value", value } });

    if(this->options.exists(logLimit)) {
        if(std::regex_search(value, regex))
            throw error;

        try {
            Console::logLimit = std::stoi(value);
        } catch(std::invalid_argument excep) {
            throw error;
        } catch(std::out_of_range excep) {
            throw error;
        }
    }
}
