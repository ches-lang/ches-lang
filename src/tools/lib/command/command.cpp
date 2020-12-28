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


typedef std::function<void(ches::PropMap&, ches::PropMap&)> cmd_proc;
typedef std::unordered_map<std::string, cmd_proc>           cmd_proc_map;


namespace ches {
    enum CommandErrorType {
        CommandError_DuplicatedOptionName,
        CommandError_InvalidOptionName,
        CommandError_InvalidOptionValue,
        CommandError_UnknownOptionName,
        CommandError_UnknownSubCommand
    };


    class CommandError : std::exception {
    public:
        CommandErrorType type;
        log_detail_map details;

        CommandError();

        CommandError(CommandErrorType type);

        CommandError(CommandErrorType type, log_detail_map details);
    };


    struct CommandOptionKeys {
        static std::string logLimit;
    };


    struct SettingKeys {
        static std::string lang;
        static std::string logLimit;
    };


    class Command {
    public:
        std::vector<std::string> args;
        std::string name;
        PropMap options;
        cmd_proc_map procMap;

        bool usedDefaultName = false;

        static std::string settingFilePath;
        PropMap settings;

        Command();

        // excep: CommandError
        Command(int argc, char* argv[], std::string defaultCmdName = "");

        inline void addProc(std::string name, cmd_proc proc) {
            this->procMap[name] = proc;
        }

        inline bool existsOptName(std::string optName) {
            return this->options.count(optName) == 1;
        }

        // excep: CommandError
        void run();

    private:
        std::string getCmdName(std::string defaultCmdName);

        PropMap getCmdOptions();

        void proceedLogLimitOption();
    };
}
