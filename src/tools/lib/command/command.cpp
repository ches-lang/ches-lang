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


typedef std::unordered_map<std::string, std::string>    cmd_options;


namespace ches {
    enum CommandErrorType {
        CommandError_DuplicatedOptionName,
        CommandError_InvalidArgument
    };


    class CommandError : std::exception {
    public:
        CommandErrorType type;

        CommandError();

        CommandError(CommandErrorType type);
    };


    // note: ここでの cmd はサブコマンドを表す
    class Command {
    public:
        std::vector<std::string> args;
        std::string name;
        cmd_options options;

        bool usedDefaultName = false;

        Command();

        // excep: CommandError
        Command(int argc, char* argv[], std::string defaultCmdName = "");

        inline bool existsOptName(std::string optName) {
            return this->options.count(optName) == 1;
        }

        // excep: CommandError
        void run();

    private:
        std::string getCmdName(std::string defaultCmdName);

        cmd_options getCmdOptions();
    };
}
