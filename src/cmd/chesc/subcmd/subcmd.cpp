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


namespace ches::cmd::chesc {
    class ChescCommand : public SubCommands {
    public:
        ChescCommand(Command cmd);

    private:
        void init() override;

        static void cmd_cmp(Command cmd) {
            std::cout << "cmp" << std::endl;
        }

        static void cmd_help(Command cmd) {
            std::cout << "help" << std::endl;
        }
    };
}
