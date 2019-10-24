#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "command.cpp"
#include "compiler.cpp"
#include "console.cpp"
#include "parser.cpp"



void command(std::string cmd) {
    typedef std::map<std::string, void(*)()> cmdprocs;
    cmdprocs procs;

    procs.insert(std::make_pair("ches", c_ches));
    procs.insert(std::make_pair("help", c_help));
    procs.insert(std::make_pair("comp", c_comp));

    cmdprocs::iterator it = procs.find(cmd);
    if(it != procs.end())
        it->second();
    else
        error("command '" + cmd + "' is not exist");
}

int main(int argc, char *argv[]) {
    std::string cmd = (argc >= 2 && argv[1][0] != '-') ? argv[1] : "ches";
    for(int i = 1; i < argc; i++)
        args.push_back(argv[i]);
    command(cmd);
}
