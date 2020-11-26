#pragma once

#include "command.hpp"


ches::Command g_cmd;


ches::Command::Command() {}

ches::Command::Command(std::vector<std::string> args) {
    ches::Console::loadLangPack("ja", "en");

    // コマンド名を設定
    this->cmdName = ((args.size() >= 2 && args.at(1)[0] != '-') ? args.at(1) : DEFAULT_CMD_NAME);

    // コマンド引数を設定
    int start = this->cmdName == DEFAULT_CMD_NAME ? 1 : 2;

    for(int i = start; i < args.size(); i++) {
        if(args.at(i)[0] != '-') {
            Console::log(LogType_Error, 8194, { { "At", "'" + std::string { args.at(i) } + "'" }}, true);
        } else if(i + 1 < args.size() && args.at(i + 1)[0] != '-') {
            this->cmdArgs[args.at(i)] = std::string { args.at(i + 1) };
            i++;
        } else {
            this->cmdArgs[std::string { args.at(i) }] = "";
        }
    }

    this->setDebugMode();
    this->setDisplayLimit();
}

std::string ches::Command::argKeyAt(std::string key) {
    return this->cmdArgs.at(key);
}

bool ches::Command::existsArgKey(std::string key) {
    return this->cmdArgs.find(key) != this->cmdArgs.end();
}

void ches::Command::run(cmdprocs procs) {
    cmdprocs::iterator itr = procs.find(g_cmd.cmdName);

    if(itr != procs.end()) {
        itr->second();
    } else {
        ches::Console::log(ches::LogType_Error, 1064, { { "Command", g_cmd.cmdName } }, true);
    }
}

void ches::Command::setDebugMode() {
    ches::Console::debugMode = this->existsArgKey("-deb");
}

void ches::Command::setDisplayLimit() {
    if(!this->existsArgKey("-limit"))
        return;

    std::string input = this->argKeyAt("limit");

    for(int i = 0; i < input.length() - 1; i++) {
        if(input[0] == '0') input.erase(input.begin());
        else break;
    }

    try {
        int limit = std::stoi(input);

        if(limit <= 0)
            Console::log(LogType_Error, 3681, { { "Input", input } }, true);

        Console::displayCountLimit = limit;
    } catch(std::invalid_argument) {
        Console::log(LogType_Error, 3681, { { "Input", input } }, true);
    }
}
