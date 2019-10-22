#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "compiler.cpp"
#include "console.cpp"
#include "lexer.cpp"



std::vector<std::string> args;



void c_ches() {
    if(args.size() == 0) {
        std::cout << "show help message with 'ches -help'" << std::endl;
    }

    else if(args[0] == "-help") {
        std::cout << "-ver" << "\t" << "show chestnut version" << std::endl;
        std::cout << std::endl;
        std::cout << "comp" << "\t" << "compile selected file(s)." << std::endl;
        std::cout << "help" << "\t" << "show help message" << std::endl;
        std::cout << "help" << "\t" << "show help message" << std::endl;
        std::cout << "help" << "\t" << "show help message" << std::endl;
        std::cout << "help" << "\t" << "show help message" << std::endl;
    }

    else if(args[0] == "-ver") {
        std::cout << "version: 0.0.0 dev" << std::endl;
    }
}

void c_comp() {
    if(args.size() == 1) {
        std::cout << "show help message with 'ches comp -help'" << std::endl;
    }

    else if(args[1] == "-help") {
        std::cout << "<filepath>" << "\t" << "compile selected file" << std::endl;
        std::cout << "<dirpath>" << "\t" << "compile files in selected directory" << std::endl;
    }

    else {
        Compiler cmp;
        cmp.loadFile(args[1]);
        cmp.compile();
    }
}

void c_help() {
    std::cout << "show help message with 'ches -help'" << std::endl;
}
