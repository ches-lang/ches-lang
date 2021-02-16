/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#include "../../shared/configulation/configulation.hpp"
#include "../../shared/command/command.hpp"
#include "../../shared/console/console.hpp"

#include "./subcmd/subcmd.hpp"

#include <iostream>
#include <string>


using namespace ches::shared;
using namespace ches::cmd::chesc;


int main(int argc, char *argv[]) {
    try {
        Configulation::loadEachData();
    } catch(FileManagerException excep) {
        Console::error.print("{^configulation.error.failedToLoadConfigulationFile}", { { "{^file.word.path}", excep.target }, { "{^error.word.errorType}", std::to_string(excep.type) } }, true);
    } catch(ConfigulationException excep) {
        Console::error.print("{^configulation.error.failedToLoadConfigulationFile}", { { "{^file.word.path}", excep.target }, { "{^error.word.errorType}", std::to_string(excep.type) } }, true);
    }

    try {
        ChescCommand chescCmd(argc, argv, "cmp");
        chescCmd.run();
    } catch(CommandException excep) {
        std::cout << "CommandException (" << excep.type << ") \"" << excep.target << "\"" << std::endl;
    } catch(ConfigulationException excep) {
        std::cout << "ConfigulationException (" << excep.type << ") \"" << excep.target << "\"" << std::endl;
    } catch(std::out_of_range excep) {
        std::cout << "OutOfRangeException" << std::endl;
    } catch(std::exception excep) {
        std::cout << "OutOfRangeException: " << excep.what() << std::endl;
    } catch(...) {
        std::cout << "UnknownException" << std::endl;
    }
}
