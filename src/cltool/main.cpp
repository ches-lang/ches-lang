#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <regex>
#include <stack>
#include <unordered_map>
#include <vector>

#include "console.cpp"
#include "utility.cpp"
#include "filemanager.cpp"
#include "command.cpp"

#include "cmd/ches/ches.cpp"
#include "cmd/cmp/cmp.cpp"
#include "cmd/run/run.cpp"


void runCommand() {
    typedef std::unordered_map<std::string, void(*)()> cmdprocs;
    cmdprocs procs;

    procs.insert(std::make_pair("ches", []() {
        ches::cmd::Ches();
    }));

    procs.insert(std::make_pair("cmp", []() {
        ches::cmd::Cmp();
    }));

    procs.insert(std::make_pair("run", []() {
        ches::cmd::Run();
    }));

    cmdprocs::iterator itr = procs.find(g_cmd.cmdName);

    if(itr != procs.end()) {
        itr->second();
    } else {
        ches::Console::log(ches::LogType_Error, 1064, { { "Command", g_cmd.cmdName } }, true);
    }
}


int main(int argc, char *argv[]) {
    g_cmd = ches::Command::fromMainArgs(argc, argv);
    runCommand();
}
