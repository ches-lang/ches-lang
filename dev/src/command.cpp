#pragma once

#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "compiler.cpp"
#include "console.cpp"
#include "interpreter.cpp"



Options options;

void c_ches() {
    if(options.exists("-help")) {
        std::cout << "-help" << "\t" << "show help message" << std::endl;
        std::cout << "-ver" << "\t" << "show chestnut version" << std::endl;
        std::cout << std::endl;
        std::cout << "cmp" << "\t" << "compile specified file(s)" << std::endl;
        std::cout << "set" << "\t" << "see or change settings" << std::endl;
    }
}

void c_cmp() {
    if(options.exists("-help")) {
        std::cout << "-help" << "\t" << "show help message" << std::endl;
        std::cout << std::endl;
        std::cout << "<filepath>" << "\t" << "compile a specified file" << std::endl;
        return;
    }

    if(options.exists("-i") && options.get("-i") != "") {
        Compiler cmp(options);
        cmp.compile();
    } else {
        Console::error("cerr5899", "no input file", {}, false);
    }
}

void c_run() {
    if(options.exists("-help")) {
        std::cout << "-help" << "\t" << "show help message" << std::endl;
        return;
    }

    if(options.exists("-i") && options.get("-i") != "") {
        Interpreter itp(options);
        itp.run();
    } else {
        Console::error("cerr5899", "no input file", {}, false);
    }
}

void c_set() {

}

void command(std::string cmd, Options opt) {
    options = opt;
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
