#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "console.cpp"
#include "syntax.cpp"
#include "lexer.cpp"
#include "bytecode.cpp"
#include "filemanager.cpp"
#include "parser.cpp"
#include "compiler.cpp"
#include "interpreter.cpp"
#include "command.cpp"



int main(int argc, char *argv[]) {
    auto start = std::chrono::system_clock::now();

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    Options options;
    Console::displayCountLimit = 20;
    Console::loadLangPacks("ja", "en");
    std::string cmd = ((argc >= 2 && argv[1][0] != '-') ? argv[1] : "ches");

    for(int i = ((cmd == "ches") ? 1 : 2); i < argc; i++) {
        if(argv[i][0] != '-') {
            Console::log(LogType_Error, "8732", { { "At", "'" + std::string{ argv[i] } + "'" }}, true);
        } else if(i + 1 < argc && argv[i + 1][0] != '-') {
            options.set(argv[i], argv[i + 1]);
            i++;
        } else {
            options.set(argv[i], "");
        }
    }

    Command command;
    command.runCommand(cmd, options);

    auto end = std::chrono::system_clock::now();
    auto dur = end - start;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    //std::cout << "dec duration: " << msec << "ms" << std::endl;
    //std::cout << "hex duration: " << std::dec << msec << "ms" << std::endl;
}
