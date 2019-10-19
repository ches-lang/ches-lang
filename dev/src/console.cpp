#pragma once



void error(std::string msg) {
    std::cout << "\033[31m" << "error: " << msg << "\033[m" << std::endl;
}



void warning(std::string msg) {
    std::cout << "\033[34m" << "warning: " << msg << "\033[m" << std::endl;
}
