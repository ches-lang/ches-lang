/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


namespace ches::shared {
    enum CommandExceptionType {
        CommandException_Unknown,
        CommandException_DuplicatedCommandName,
        CommandException_DuplicatedOptionName,
        CommandException_InvalidOptionName,
        CommandException_UnexpectedOptionValue,
        CommandException_UnknownCommandName,
        CommandException_UnknownOptionName,
    };


    class CommandException : std::exception {
    public:
        CommandExceptionType type = CommandException_Unknown;
        std::string target = "";

        CommandException() noexcept;

        CommandException(CommandExceptionType type) noexcept;

        CommandException(CommandExceptionType type, std::string target) noexcept;
    };


    struct CommandOption {
    public:
        /*
         * note: 外部からの値の操作は、値チェックのある CommandOptionMap 側で行うこと
         */
        std::vector<std::string> values;

        CommandOption() noexcept;
    };


    struct CommandOptionMap : public std::unordered_map<std::string, CommandOption> {
    public:
        CommandOptionMap() noexcept;

        /*
         * excep: CommandException [UnknownOptionName]
         */
        inline CommandOption at(std::string optionName) {
            if(this->count(optionName) == 0)
                throw CommandException(CommandException_UnknownOptionName, optionName);

            return (*this)[optionName];
        }

        /*
         * CommandException [DuplicatedOptionName]
         */
        inline void addOption(std::string optionName, std::vector<std::string> optionValues = {}) {
            if(this->count(optionName) == 1)
                throw CommandException(CommandException_DuplicatedOptionName, optionName);

            CommandOption option;
            option.values = optionValues;
            (*this)[optionName] = option;
        }
    };


    struct Command {
    public:
        typedef std::function<void(Command&)>                    CommandProc;
        typedef std::unordered_map<std::string, CommandProc>    CommandProcMap;

        std::string cmdName = "";
        std::string defaultCmdName = "";

        CommandOptionMap cmdOptionMap;

    private:
        CommandProcMap cmdProcMap;

    public:
        Command() noexcept;

        /*
         * excep: Command::loadFromArgs(std::vector<std::string>)
         */
        Command(std::vector<std::string> args, std::string defaultCmdName);

        /*
         * excep: CommandException [DuplicatedCommandName]
         */
        void addCommandProc(std::string cmdName, CommandProc proc);

        void print() noexcept;

        /*
         * excep: CommandException [UnknownCommandName]
         */
        void run();

    private:
        /*
         * excep: CommandException [InvalidOptionName, UnexpectedOptionValue]
         */
        void loadFromArgs(std::vector<std::string> args);
    };


    class SubCommands {
    protected:
        Command cmd;

    public:
        virtual void init(std::vector<std::string> args, std::string defaultCmdName) = 0;

        void run() noexcept;
    };
}
