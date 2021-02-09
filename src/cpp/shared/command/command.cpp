/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


namespace ches::shared {
    enum CommandExceptionType {
        CommandException_Unknown,
        CommandException_UnknownCommandName,
        CommandException_UnknownOptionName,
        CommandException_CommandNameAlreadyExists,
        CommandException_OptionNameAlreadyExists,
        CommandException_UnexpectedOptionValue
    };


    class CommandException : std::exception {
    public:
        CommandExceptionType type = CommandException_Unknown;
        std::string target = "";

        CommandException();

        CommandException(CommandExceptionType type);

        CommandException(CommandExceptionType type, std::string target);
    };


    struct CommandOption {
    public:
        /*
         * note: 外部からの値の操作は、値チェックのある CommandOptionMap 側で行うこと
         */
        std::vector<std::string> values;

        CommandOption();
    };


    struct CommandOptionMap : public std::unordered_map<std::string, CommandOption> {
    public:
        CommandOptionMap();

        /*
         * excep: CommandException
         */
        inline CommandOption at(std::string optionName) {
            if(this->count(optionName) == 0)
                throw CommandException(CommandException_UnknownOptionName, optionName);

            return (*this)[optionName];
        }

        inline void addOption(std::string optionName, std::vector<std::string> optionValues = {}) {
            if(this->count(optionName) == 1)
                throw CommandException(CommandException_OptionNameAlreadyExists, optionName);

            CommandOption option;
            option.values = optionValues;
            (*this)[optionName] = option;
        }
    };


    struct Command {
    public:
        std::string cmdName = "";
        std::string defaultCmdName = "";

        CommandOptionMap cmdOptionMap;

        typedef std::function<void(Command)>                    CommandProc;
        typedef std::unordered_map<std::string, CommandProc>    CommandProcMap;

    private:
        CommandProcMap cmdProcMap;

    public:
        Command();

        Command(int argc, char *argv[], std::string defaultCmdName);

        Command(std::vector<std::string> args, std::string defaultCmdName);

        /*
         * except: CommandException [CommandNameAlreadyExists]
         */
        void addCommandProc(std::string cmdName, CommandProc proc);

        void print();

        /*
         * except: CommandException [UnknownCommandName]
         */
        void run();

    private:
        /*
         * except: CommandException [CommandNameAlreadyExists]
         */
        void loadFromArgs(std::vector<std::string> args);
    };


    class SubCommands {
    public:
        Command cmd;

        virtual void init() = 0;

        void run();
    };
}
