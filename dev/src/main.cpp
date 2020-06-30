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

#include "cmddata.cpp"
#include "console.cpp"
#include "utility.cpp"
#include "lexer.cpp"
#include "filemanager.cpp"
#include "parser.cpp"
#include "compiler.cpp"
#include "interpreter.cpp"
#include "command.cpp"



CommandData getCmdData(int argc, char *args[]) {
    std::string cmdName = ((argc >= 2 && args[1][0] != '-') ? args[1] : "ches");
    std::unordered_map<std::string, std::string> cmdArgs;

    for(int i = ((cmdName == "ches") ? 1 : 2); i < argc; i++) {
        if(args[i][0] != '-') {
            Console::log(LogType_Error, 8732, { { "At", "'" + std::string { args[i] } + "'" }}, true);
        } else if(i + 1 < argc && args[i + 1][0] != '-') {
            cmdArgs[args[i]] = std::string { args[i + 1] };
            i++;
        } else {
            cmdArgs[std::string { args[i] }] = "";
        }
    }

    return CommandData(cmdName, cmdArgs);
}



int main(int argc, char *argv[]) {
    auto start = std::chrono::system_clock::now();

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    Console::displayCountLimit = 20;
    Console::loadLangPacks("ja", "en");

    g_cmd_data = getCmdData(argc, argv);

    // 開始時のログを出力
    if(g_cmd_data.exists("-deb")) {
        Console::writeln();
        Console::printDebugLog("start debugger");
    }

    Command command;
    command.runCommand();

    auto end = std::chrono::system_clock::now();
    auto dur = end - start;
    auto msec = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();

    // 計測時間のログを出力
    Console::printDebugLog("process duration", { "whole: " + std::to_string(msec) + "µs" });

    // 終了時のログを出力
    Console::printDebugLog("finish debugger");
}
