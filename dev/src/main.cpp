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

    Options options;
    std::string cmd = ((argc >= 2 && argv[1][0] != '-') ? argv[1] : "ches");

    for(int i = ((cmd == "ches") ? 1 : 2); i < argc; i++) {
        if(argv[i][0] != '-') {
            Console::error("cerr8732", "invalid command option", { { "at", "'" + std::string{ argv[i] } + "'" }}, true);
        } else if(i + 1 < argc && argv[i + 1][0] != '-') {
            options.set(argv[i], argv[i + 1]);
            i++;
        } else {
            options.set(argv[i], "");
        }
    }

    command(cmd, options);

    auto end = std::chrono::system_clock::now();
    auto dur = end - start;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    std::cout << msec << "ms" << std::endl;
}
