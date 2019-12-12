#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "command.cpp"
#include "compiler.cpp"
#include "console.cpp"



int main(int argc, char *argv[]) {
    auto start = std::chrono::system_clock::now();

    std::vector<std::vector<int>> v = {};
    std::cout << v.size() << std::endl;

    std::map<std::string, std::string> args;
    std::string cmd = ((argc >= 2 && argv[1][0] != '-') ? argv[1] : "ches");

    for(int i = ((cmd == "ches") ? 1 : 2); i < argc; i++) {
        if(i + 1 < argc) {
            if(argv[i + 1][0] != '-') {
                args[argv[i]] = argv[i + 1];
                i++;
            } else {
                args[argv[i]] = "";
            }
        } else if(argv[i][0] == '-') {
            args[argv[i]] = "";
        } else {
            Console::error("cerr0000", "invalid command option", {}, true);
        }
    }

    command(cmd, args);

    auto end = std::chrono::system_clock::now();
    auto dur = end - start;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    std::cout << msec << "ms" << std::endl;
}
