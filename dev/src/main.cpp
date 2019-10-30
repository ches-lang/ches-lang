#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "command.cpp"
#include "compiler.cpp"
#include "console.cpp"



std::vector<std::pair<std::string, std::string>> args;



int main(int argc, char *argv[]) {
    std::string cmd = (argc >= 2 && argv[1][0] != '-') ? argv[1] : "ches";
    int i = (cmd == "ches") ? 1 : 2;
    for(; i < argc; i++)
        if(argv[i][0] == '-')
            if(argc > i + 1)
                { args.push_back(std::make_pair(argv[i], argv[i + 1])); i++; }
            else
                args.push_back(std::make_pair(argv[i], ""));
        else
            args.push_back(std::make_pair("", argv[i]));

    command(cmd);
}
