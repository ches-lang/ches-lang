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


typedef std::unordered_map<std::string, std::string>    cmd_opt_map;
typedef std::function<void(cmd_opt_map)>                cmd_proc;
typedef std::unordered_map<std::string, cmd_proc>       cmd_proc_map;


namespace ches {
    enum CommandErrorType {
        CommandError_DuplicatedOptionName,
        CommandError_InvalidArgument,
        CommandError_UnknownSubCommand
    };


    class CommandError : std::exception {
    public:
        CommandErrorType type;

        CommandError();

        CommandError(CommandErrorType type);
    };


    class Command {
    public:
        std::vector<std::string> args;
        std::string name;
        cmd_opt_map optMap;
        cmd_proc_map procMap;

        bool usedDefaultName = false;

        Command();

        // excep: CommandError
        Command(int argc, char* argv[], std::string defaultCmdName = "");

        inline void addProc(std::string name, cmd_proc proc) {
            this->procMap[name] = proc;
        }

        inline bool existsOptName(std::string optName) {
            return this->optMap.count(optName) == 1;
        }

        // excep: CommandError
        void run();

    private:
        std::string getCmdName(std::string defaultCmdName);

        cmd_opt_map getCmdOptions();
    };
}
