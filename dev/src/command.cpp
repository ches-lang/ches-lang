#pragma once

#include "command.hpp"



Command::Command() {}

void Command::runCommand() {
    typedef std::map<std::string, void(*)()> cmdprocs;
    cmdprocs procs;

    if(g_cmd_data.exists("-limit")) {
        std::string input = g_cmd_data["-limit"];

        for(int i = 0; i < input.length() - 1; i++) {
            if(input[0] == '0') input.erase(input.begin());
            else break;
        }

        try {
            int limit = std::stoi(input);

            if(limit <= 0)
                Console::log(LogType_Error, "3681", { { "Input", input } }, true);

            Console::displayCountLimit = limit;
        } catch(std::invalid_argument) {
            Console::log(LogType_Error, "3681", { { "Input", input } }, true);
        }
    }

    procs.insert(std::make_pair("ches", Command::c_ches));
    procs.insert(std::make_pair("cmp", Command::c_cmp));
    procs.insert(std::make_pair("run", Command::c_run));
    procs.insert(std::make_pair("set", Command::c_set));

    cmdprocs::iterator itr = procs.find(g_cmd_data.cmdName);

    if(itr != procs.end()) {
        itr->second();
    } else {
        Console::log(LogType_Error, "1064", { { "Command", g_cmd_data.cmdName } }, true);
    }
}
