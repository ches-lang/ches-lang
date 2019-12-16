#pragma once

#include <iostream>
#include <map>
#include <string>



class Console {

public:

    static bool errored;
    static bool warned;
    static bool noticed;

    static void error(std::string code, std::string msg, std::map<std::string, std::string> details = {}, bool terminate = false) {
        errored = true;
        std::cout << "\033[31m" << "|" << code << "| " << msg << std::endl;

        for(std::pair<std::string, std::string> p : details)
            std::cout << "\t" << p.first << ": " << p.second << std::endl;

        std::cout << "\033[m";

        if(terminate) exit(-1);
    }

    static void warn(std::string code, std::string msg, std::map<std::string, std::string> details = {}, bool terminate = false) {
        warned = true;
        std::cout << "\033[35m" << "|" << code << "| " << msg << std::endl;

        for(std::pair<std::string, std::string> p : details)
            std::cout << "\t" << p.first << ": " << p.second << std::endl;

        std::cout << "\033[m";

        Console::notice("cnote4247", "[-miss] to continue forcibly", {}, false);

        if(terminate) exit(-1);
    }

    static void notice(std::string code, std::string msg, std::map<std::string, std::string> details = {}, bool terminate = false) {
        noticed = true;
        std::cout << "\033[36m" << "|" << code << "| " << msg << std::endl;

        for(std::pair<std::string, std::string> p : details)
            std::cout << "\t" << p.first << ": " << p.second << std::endl;

        std::cout << "\033[m";

        if(terminate) exit(-1);
    }

};

bool Console::errored;
bool Console::warned;
bool Console::noticed;
