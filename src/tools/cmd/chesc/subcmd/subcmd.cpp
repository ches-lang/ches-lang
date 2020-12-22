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


#pragma once


namespace ches {
    class ChesSubCommands {
    public:
        static void addAllCommands(Command &cmd) {
            cmd.addProc("cmp", ChesSubCommands::cmd_cmp);
        }

        static void cmd_cmp(cmd_opt_map &optMap, Settings &settings) {
            try {
                std::cout << "cmp" << std::endl;
            } catch(FileError excep) {
                std::cout << "fileerror (" << excep.type << ")" << std::endl;
            }
        }
    };
}
