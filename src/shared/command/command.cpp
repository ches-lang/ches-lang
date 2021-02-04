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


namespace ches::shared {
    enum CommandExceptionType {
        CommandException_Unknown,
        CommandException_UnknownOptionName,
        CommandException_InvalidOptionName,
        CommandException_OptionNameAlreadyExists
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
            if(!this->exists(optionName))
                throw CommandException(CommandException_UnknownOptionName, optionName);

            return (*this)[optionName];
        }

        inline void addOption(std::string optionName, std::vector<std::string> optionValues = {}) {
            if(this->exists(optionName))
                throw CommandException(CommandException_OptionNameAlreadyExists, optionName);

            CommandOption option;
            option.values = optionValues;
            (*this)[optionName] = option;
        }

        inline bool exists(std::string optionName) {
            return this->count(optionName) == 1;
        }
    };


    struct Command {
    public:
        std::string cmdName = "";
        CommandOptionMap optionMap;

        std::string defaultCmdName = "";
        std::string defaultOptionName = "";

        Command();

        Command(int argc, char *argv[], std::string defaultCmdName, std::string defaultOptionName);

        Command(std::vector<std::string> args, std::string defaultCmdName, std::string defaultOptionName);

        void print();

    private:
        void loadFromArgs(std::vector<std::string> args);
    };
}
