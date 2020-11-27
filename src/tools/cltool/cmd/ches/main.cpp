#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <regex>
#include <stack>
#include <unordered_map>
#include <vector>

#include "../../enums/enums.hpp"
#include "../../console/console.hpp"
#include "../../filemanager/filemanager.hpp"
#include "../../command/command.hpp"

#include "./interpreter/interpreter.hpp"
#include "./cmdlist/cmdlist.hpp"


cmdprocs getCommandProcs() {
    cmdprocs procs;

    procs.insert(std::make_pair(DEFAULT_CMD_NAME, []() {
        ches::cmd::Ches::cmd_default();
    }));

    return procs;
}


int main(int argc, char *argv[]) {
    g_cmd = ches::Command::fromMainArgs(argc, argv);
    cmdprocs procs = getCommandProcs();
    g_cmd.run(procs);
}
