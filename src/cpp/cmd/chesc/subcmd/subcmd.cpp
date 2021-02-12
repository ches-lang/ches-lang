/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


namespace ches::cmd::chesc {
    class ChescCommand : public SubCommands {
    public:
        ChescCommand(int argc, char* argv[], std::string defaultCmdName);

    private:
        void init(std::vector<std::string> args, std::string defaultCmdName) override;

        static void cmd_cmp(Command cmd) {
            std::cout << "cmp" << std::endl;
        }

        static void cmd_help(Command cmd) {
            std::cout << "help" << std::endl;
        }
    };
}
