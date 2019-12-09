#pragma once

#include <iostream>
#include <string>



class Console {

public:

    static bool errored;
    static bool warned;
    static bool noticed;

    static void error(std::string code, std::string msg, std::map<std::string, std::string> details = {}, bool terminate = false) {
        errored = true;
        std::cout << "\033[31m" << "|" << code << "| ";
        std::cout << msg << std::endl;
        for(std::pair<std::string, std::string> p : details)
            std::cout << "\t" << p.first << ": " << p.second << std::endl;
        std::cout << "\033[m";
        if(terminate) exit(-1);
    }

    static void warn() {
        warned = true;
    }

    static void notice(std::string msg, bool terminate) {
        noticed = true;
        std::cout << "\033[36mnotice: " << msg << "\033[m" << std::endl;
        if(terminate) exit(-1);
    }

};

bool Console::errored;
bool Console::warned;
bool Console::noticed;
