#include <iostream>
#include <map>
#include <string>
#include <vector>

#define length(array) (sizeof(array) / sizeof(array[0]))



void error(std::string msg) {
    std::cout << "\033[31m" << "error: " << msg << "\033[m" << std::endl;
}

void warning(std::string msg) {
    std::cout << "\033[34m" << "warning: " << msg << "\033[m" << std::endl;
}



std::vector<std::string> args;

void c_ches() {
    std::cout << std::endl;

    if(args.size() == 0) {
        std::cout << "show help message with 'ches help'" << std::endl;
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

    std::cout << std::endl;
}

void c_comp() {
    std::cout << std::endl;

    if(args.size() == 1) {
        std::cout << "show help message with 'ches comp -help'" << std::endl;
    }

    else if(args[1] == "-help") {
        std::cout << "<filepath>" << "\t" << "compile selected file" << std::endl;
        std::cout << "<dirpath>" << "\t" << "compile files in selected directory" << std::endl;
    }

    std::cout << std::endl;
}

void c_help() {
    std::cout << std::endl;
    std::cout << "show help message with 'ches -help'" << std::endl;
    std::cout << std::endl;
}



void command(std::string cmd) {
    typedef std::map<std::string, void(*)()> cmdprocs;
    cmdprocs procs;

    procs.insert(std::make_pair("ches", c_ches));
    procs.insert(std::make_pair("help", c_help));
    procs.insert(std::make_pair("comp", c_comp));



    cmdprocs::iterator it = procs.find(cmd);
    if(it != procs.end())
        it->second();
    else
        error("command '" + cmd + "' is not exist");
}



int main(int argc, char *argv[]) {
    std::string cmd = (argc >= 2 && argv[1][0] != '-') ? argv[1] : "ches";
    for(int i = 1; i < argc; i++)
        args.push_back(argv[i]);
    command(cmd);
}
