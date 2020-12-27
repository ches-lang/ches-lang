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

#include "../datastruct/datastruct.hpp"

#include "./command.cpp"


ches::CommandError::CommandError() {}

ches::CommandError::CommandError(CommandErrorType type) {
    this->type = type;
}


std::string ches::Command::settingFilePath = "./src/tools/cmd/chesc/data/settings.pmap";

ches::Command::Command() {}

ches::Command::Command(int argc, char* argv[], std::string defaultCmdName) {
    // note: コマンド名 (最初の引数) をスキップ
    for(int i = 1; i < argc; i++)
        this->args.push_back(std::string(argv[i]));

    this->name = this->getCmdName(defaultCmdName);
    this->options = this->getCmdOptions();

    // proc: 設定ファイルのロード

    this->settings.load(Command::settingFilePath);

    // proc: 言語コードの変更

    std::string langPropKey = "lang";

    if(this->settings.exists(langPropKey))
        Console::langCode = this->settings.get(langPropKey);

    // proc: -setオプションのロード

    if(this->options.exists("set")) {
        try {
            this->proceedSetOption(this->options.at("set"));
        } catch(CommandError excep) {
            throw excep;
        }
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

// note: optionValueと同じ形式の文字列を扱う必要がある場合は、他の関数に解析処理をまとめること
// arg (optionValue): "a: b, c: d ..." というような形式の文字列
// excep: CommandError
void ches::Command::proceedSetOption(std::string optionValue) {
    while(true) {
        int endIndex = optionValue.find(",");
        // note: セパレータが見つからない場合は第二引数にstd::string::nposを渡す
        std::string option = optionValue.substr(0, endIndex);

        int sepIndex = option.find(":");

        // note: ペア内に":"がない場合はエラー
        if(sepIndex == std::string::npos)
            throw CommandError(CommandError_InvalidOptionValue);

        std::string settingName = trim_string_spaces(option.substr(0, sepIndex));
        std::string settingValue = trim_string_spaces(option.substr(sepIndex + 1));

        if(settingName.size() == 0 || settingValue.size() == 0)
            throw CommandError(CommandError_InvalidOptionValue);

        if(!this->settings.exists(settingName))
            throw CommandError(CommandError_UnknownOptionName);

        this->settings[settingName] = settingValue;

        // note: これ以上ペアがなければbreak
        if(endIndex == std::string::npos)
            break;

        optionValue = optionValue.substr(endIndex + 1);
    }
}
