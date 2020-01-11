#pragma once

#include <iostream>
#include <string>
#include <unordered_map>



class Console {

public:

    static int displayCount;
    static int displayCountLimit;   // No limit: 0

    static bool errored;
    static bool warned;
    static bool noticed;

    static void error(std::string code, std::string msg, std::unordered_map<std::string, std::string> details = {}, bool terminate = false) {
        if(errored || warned || noticed) std::cout << std::endl;

        errored = true;
        displayCount++;

        std::cout << "\033[31m" << "|" << code << "|" << "\033[m" << " " << msg << std::endl;

        for(std::pair<std::string, std::string> p : details)
            std::cout << "\t" << p.first << ": " << p.second << std::endl;

        if(displayCount == displayCountLimit)
            Console::notice("cnote7148", "[-limit] to change display limit", { { "limit", std::to_string(displayCountLimit) } }, true);

        if(terminate) exit(-1);
    }

    static void warn(std::string code, std::string msg, std::unordered_map<std::string, std::string> details = {}, bool terminate = false) {
        if(errored || warned || noticed) std::cout << std::endl;

        warned = true;
        displayCount++;

        std::cout << "\033[35m" << "|" << code << "|" << "\033[m" << " " << msg << std::endl;

        for(std::pair<std::string, std::string> p : details)
            std::cout << "\t" << p.first << ": " << p.second << std::endl;

        Console::notice("cnote4247", "[-miss] to continue forcibly", {}, false);

        if(displayCount == displayCountLimit)
            Console::notice("cnote7148", "[-limit] to change display limit", { { "limit", std::to_string(displayCountLimit) } }, true);

        if(terminate) exit(-1);
    }

    static void notice(std::string code, std::string msg, std::unordered_map<std::string, std::string> details = {}, bool terminate = false) {
        if(errored || warned || noticed) std::cout << std::endl;

        noticed = true;
        displayCount++;

        std::cout << "\033[36m" << "|" << code << "|" << "\033[m" << " " << msg << std::endl;

        for(std::pair<std::string, std::string> p : details)
            std::cout << "\t" << p.first << ": " << p.second << std::endl;

        if(displayCount == displayCountLimit)
            Console::notice("cnote7148", "[-limit] to change display limit", { { "limit", std::to_string(displayCountLimit) } }, true);

        if(terminate) exit(-1);
    }

};

int Console::displayCount;
int Console::displayCountLimit;

bool Console::errored;
bool Console::warned;
bool Console::noticed;
