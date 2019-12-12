#pragma once

#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "compiler.cpp"
#include "console.cpp"
#include "interpreter.cpp"



std::map<std::string, std::string> args;



void c_ches() {
    if(args.find("-help") != args.end()) {
        std::cout << "-help" << "\t" << "show help message" << std::endl;
        std::cout << "-ver" << "\t" << "show chestnut version" << std::endl;
        std::cout << std::endl;
        std::cout << "cmp" << "\t" << "compile specified file(s)" << std::endl;
        std::cout << "set" << "\t" << "see or change settings" << std::endl;
    }
}

void c_cmp() {
    if(args.find("-help") != args.end()) {
        std::cout << "-help" << "\t" << "show help message" << std::endl;
        std::cout << std::endl;
        std::cout << "<filepath>" << "\t" << "compile a specified file" << std::endl;
        return;
    }

    if(args.find("-i") != args.end()) {std::cout<<"a"<<std::endl;
        Compiler cmp(args);std::cout<<"b"<<std::endl;
        cmp.compile();std::cout<<"c"<<std::endl;
    } else {
        Console::error("cerr0000", "no input file", {{}}, false);
    }
}

void c_run() {
    if(args.find("-help") != args.end()) {
        std::cout << "-help" << "\t" << "show help message" << std::endl;
        return;
    }

    if(args.size() >= 1) {
        Interpreter itp(args[""]);
        itp.run();
    }
}

void c_set() {

}

void command(std::string cmd, std::map<std::string, std::string> ag) {
    args = ag;
    typedef std::map<std::string, void(*)()> cmdprocs;
    cmdprocs procs;

    procs.insert(std::make_pair("ches", c_ches));
    procs.insert(std::make_pair("cmp", c_cmp));
    procs.insert(std::make_pair("run", c_run));
    procs.insert(std::make_pair("set", c_set));

    cmdprocs::iterator it = procs.find(cmd);
    if(it != procs.end())
        it->second();
    else
        Console::error("cerr1064", "unknown command", { { "command", cmd } }, true);
}
