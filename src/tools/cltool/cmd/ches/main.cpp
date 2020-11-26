#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <regex>
#include <stack>
#include <unordered_map>
#include <vector>

#include "../../console/console.cpp"
#include "../../filemanager/filemanager.cpp"
#include "../../command/command.cpp"

#include "./cmdlist/cmdlist.cpp"


typedef std::unordered_map<std::string, void(*)()> cmdprocs;


void runCommand() {
    cmdprocs procs;

    procs.insert(std::make_pair("run", []() {
        ches::run::Run();
    }));

    ches::Command::run(procs);
}


int main(int argc, char *argv[]) {
    g_cmd = ches::Command::fromMainArgs(argc, argv);
    runCommand();
}
