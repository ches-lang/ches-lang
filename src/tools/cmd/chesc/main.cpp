/* 
 * 
 * Chestnut 0.0.0
 * 
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 * 
 * Copyright © 2020 Garnet3106 All rights reserved. 
 * 
 */


#include <iostream>

#include "../../lib/console/console.hpp"
#include "../../lib/command/command.hpp"

#include "./subcmd/subcmd.hpp"


int main(int argc, char *argv[]) {
    try {
        ches::LangPack::data.load("./src/tools/cmd/chesc/data/langpack.pmap");
        ches::Settings::data.load("./src/tools/cmd/chesc/data/settings.pmap");

        ches::LangPack::langCode = ches::Settings::data.get("lang");
        ches::Console::logLimitLogNum = ches::Note_LogLimit;

        ches::Command cmd(argc, argv, "cmp");
        ches::ChesSubCommands::addAllCommands(cmd);

        ches::Console::log(ches::LogType_Warn, ches::Warn_UnknownWarning);
        ches::Console::log(ches::LogType_Note, ches::Warn_UnknownWarning);
        ches::Console::log(ches::LogType_Error, ches::Warn_UnknownWarning);

        cmd.run();
    } catch(ches::CommandError excep) {
        // todo: Consoleを実装してから例外処理を改良する
        std::cout << "command error (" << excep.type << ")" << std::endl;
    } catch(std::exception excep) {
        std::cout << "unknown error" << std::endl;
    }
}
