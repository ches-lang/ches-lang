#pragma once

#define timepoint   std::chrono::_V2::system_clock::time_point

#include "command.hpp"


Command::Command() {}

Command::Command(int argc, char *argv[]) {
    // 開始時のログを出力
    if(this->existsKey("-deb")) {
        Console::writeln();
        Console::printDebugLog("start debugger");
    }

    this->cmdName = ((argc >= 2 && argv[1][0] != '-') ? argv[1] : "ches");
    this->setArgs(argc, argv);
}

void Command::setArgs(int argc, char *argv[]) {
    for(int i = ((this->cmdName == "ches") ? 1 : 2); i < argc; i++) {
        if(argv[i][0] != '-') {
            Console::log(LogType_Error, 8732, { { "At", "'" + std::string { argv[i] } + "'" }}, true);
        } else if(i + 1 < argc && argv[i + 1][0] != '-') {
            (*this)[argv[i]] = std::string { argv[i + 1] };
            i++;
        } else {
            (*this)[std::string { argv[i] }] = "";
        }
    }
}

timepoint Command::startCount() {
    timepoint start = std::chrono::system_clock::now();

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    return start;
}

void Command::endCount(timepoint start) {
    auto end = std::chrono::system_clock::now();
    auto dur = end - start;
    auto msec = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();

    // 計測時間のログを出力
    Console::printDebugLog("process duration", { "whole: " + std::to_string(msec) + "µs" });
}

bool Command::existsKey(std::string key) {
    return this->find(key) != this->end();
}

void Command::run() {
    if(this->existsKey("-limit")) {
        std::string input = this->at("-limit");

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

    switch(this->cmdName) {
        case "cmp":

        break;

        default:
        Console::log(LogType_Error, 1064, { { "Command", this->cmdName } }, true);
    }
}
