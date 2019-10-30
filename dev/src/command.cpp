#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "compiler.cpp"
#include "console.cpp"



extern std::vector<std::pair<std::string, std::string>> args;



int find(std::string name) {
    for(int i = 0; i < args.size(); i++)
        if(args[i].first == name)
            return i;
    return -1;
}



void c_ches() {
    if(find("-help") != -1) {
        std::cout << "-help" << "\t" << "show help message" << std::endl;
        std::cout << "-ver" << "\t" << "show chestnut version" << std::endl;
        std::cout << std::endl;
        std::cout << "cmp" << "\t" << "compile specified file(s)" << std::endl;
        std::cout << "set" << "\t" << "see or change settings" << std::endl;
    }
}

void c_cmp() {
    if(find("-help") != -1) {
        std::cout << "-help" << "\t" << "show help message" << std::endl;
        std::cout << std::endl;
        std::cout << "<filepath>" << "\t" << "compile a specified file" << std::endl;
        return;
    }

    if(args.size() >= 1) {
        Compiler cmp;
        cmp.load(args[0].second);
        cmp.run();
    }
}

void c_set() {

}



void command(std::string cmd) {
    typedef std::map<std::string, void(*)()> cmdprocs;
    cmdprocs procs;

    procs.insert(std::make_pair("ches", c_ches));
    procs.insert(std::make_pair("cmp", c_cmp));
    procs.insert(std::make_pair("set", c_set));

    cmdprocs::iterator it = procs.find(cmd);
    if(it != procs.end())
        it->second();
    else
        error("unknown command '" + cmd + "'", true);
}
