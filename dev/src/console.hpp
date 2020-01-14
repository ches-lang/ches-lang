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
        if(Console::errored || Console::warned || Console::noticed) std::cout << std::endl;

        Console::errored = true;
        Console::displayCount++;

        std::cout << "\033[31m" << "|" << code << "|" << "\033[m" << " " << msg << std::endl;

        for(auto det : details)
            std::cout << "\t" << det.first << ": " << det.second << std::endl;

        if(Console::displayCount == Console::displayCountLimit)
            Console::notice("cnote7148", "[-limit] to change display limit", { { "limit", std::to_string(Console::displayCountLimit) } }, true);

        if(terminate) exit(-1);
    }

    static void warn(std::string code, std::string msg, std::unordered_map<std::string, std::string> details = {}, bool terminate = false) {
        if(Console::errored || Console::warned || Console::noticed) std::cout << std::endl;

        Console::warned = true;
        Console::displayCount++;

        std::cout << "\033[35m" << "|" << code << "|" << "\033[m" << " " << msg << std::endl;

        for(std::pair<std::string, std::string> det : details)
            std::cout << "\t" << det.first << ": " << det.second << std::endl;

        Console::notice("cnote4247", "[-miss] to continue forcibly", {}, false);

        if(Console::displayCount == Console::displayCountLimit)
            Console::notice("cnote7148", "[-limit] to change display limit", { { "limit", std::to_string(Console::displayCountLimit) } }, true);

        if(terminate) exit(-1);
    }

    static void notice(std::string code, std::string msg, std::unordered_map<std::string, std::string> details = {}, bool terminate = false) {
        if(Console::errored || Console::warned || Console::noticed) std::cout << std::endl;

        Console::noticed = true;
        Console::displayCount++;

        std::cout << "\033[36m" << "|" << code << "|" << "\033[m" << " " << msg << std::endl;

        for(std::pair<std::string, std::string> det : details)
            std::cout << "\t" << det.first << ": " << det.second << std::endl;

        if(Console::displayCount == Console::displayCountLimit)
            Console::notice("cnote7148", "[-limit] to change display limit", { { "limit", std::to_string(Console::displayCountLimit) } }, true);

        if(terminate) exit(-1);
    }
};

int Console::displayCount;
int Console::displayCountLimit;

bool Console::errored;
bool Console::warned;
bool Console::noticed;
