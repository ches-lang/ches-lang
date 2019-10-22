#pragma once



void error(std::string msg) {
    std::cout << "\033[31m" << "error: " << msg << "\033[m" << std::endl;
}

void error(std::string msg, bool terminate) {
    std::cout << "\033[31m" << "error: " << msg << "\033[m" << std::endl;
    if(terminate) exit(-1);
}

void warning(std::string msg) {
    std::cout << "\033[34m" << "warning: " << msg << "\033[m" << std::endl;
}
