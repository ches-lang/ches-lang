#pragma once

#include <iostream>
#include <string>



void error(std::string msg, bool terminate) {
    std::cout << "\033[31merror: " << msg << "\033[m" << std::endl;
    if(terminate) exit(-1);
}

void error(std::string code, std::string msg, std::vector<std::string> details, bool terminate) {
    if(details.size() % 2 != 0) return;
    std::cout << "\033[31m" << code <<": ";
    std::cout << msg << std::endl;
    for(int i = 0; i < details.size(); i += 2)
        std::cout << "\t" << details[i] << ": " << details[i + 1] << std::endl;
    std::cout << "\033[m";
    if(terminate) exit(-1);
}

void notice(std::string msg, bool terminate) {
    std::cout << "\033[36mnotice: " << msg << "\033[m" << std::endl;
    if(terminate) exit(-1);
}
