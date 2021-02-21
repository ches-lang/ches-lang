/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#include "../../shared/configuration/configuration.hpp"
#include "../../shared/command/command.hpp"
#include "../../shared/console/console.hpp"

#include "./subcmd/subcmd.hpp"

#include <iostream>
#include <string>


using namespace ches::cmd::chesc;
using namespace ches::compiler;
using namespace ches::shared;


int main(int argc, char *argv[]) {
    try {
        Configuration::loadEachData();
    } catch(FileManagerException excep) {
        Console::error.print("{^config.error.failedToReadConfigFile}", { { "{^file.words.path}", excep.target }, { "{^general.words.errorType}", "FileManagerException [" + std::to_string(excep.type) + "]" } }, true);
    } catch(ConfigurationException excep) {
        Console::error.print("{^config.error.failedToParseConfigFile}", { { "{^file.words.path}", excep.target }, { "{^error.words.errorType}", "ConfigurationException [" + std::to_string(excep.type) + "]" } }, true);
    }

    try {
        ChescCommand chescCmd(argc, argv, "cmp");
        chescCmd.run();
    } catch(CommandException excep) {
        std::cout << "CommandException (" << excep.type << ") \"" << excep.target << "\"" << std::endl;
    } catch(ConfigurationException excep) {
        std::cout << "ConfigurationException (" << excep.type << ") \"" << excep.target << "\"" << std::endl;
    } catch(std::out_of_range excep) {
        std::cout << "OutOfRangeException" << std::endl;
    } catch(std::exception excep) {
        std::cout << "OutOfRangeException: " << excep.what() << std::endl;
    } catch(...) {
        Console::error.print("{^general.error.unknownError}", true);
    }
}
