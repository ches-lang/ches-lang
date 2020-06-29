#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <random>
#include <regex>
#include <stack>
#include <string>
#include <unordered_map>
#include <uuid/uuid.h>
#include <vector>

#include "console.cpp"
#include "utility.cpp"
#include "lexer.cpp"
#include "filemanager.cpp"
#include "parser.cpp"
#include "compiler.cpp"
#include "interpreter.cpp"
#include "command.cpp"



int main(int argc, char *argv[]) {
    auto start = std::chrono::system_clock::now();

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    // 開始時のログを出力
    Console::printDebugLog("start debugger");

    Options options;
    Console::displayCountLimit = 20;
    Console::loadLangPacks("ja", "en");
    std::string cmd = ((argc >= 2 && argv[1][0] != '-') ? argv[1] : "ches");

    for(int i = ((cmd == "ches") ? 1 : 2); i < argc; i++) {
        if(argv[i][0] != '-') {
            Console::log(LogType_Error, "8732", { { "At", "'" + std::string{ argv[i] } + "'" }}, true);
        } else if(i + 1 < argc && argv[i + 1][0] != '-') {
            options[argv[i]] = argv[i + 1];
            i++;
        } else {
            options[argv[i]] = "";
        }
    }

    Command command;
    command.runCommand(cmd, options);

    auto end = std::chrono::system_clock::now();
    auto dur = end - start;
    auto msec = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();

    // 計測時間のログを出力
    Console::printDebugLog("process duration", { "whole: " + std::to_string(msec) + "µs" });

    // 終了時のログを出力
    Console::printDebugLog("finish debugger");
    Console::writeln();
}
