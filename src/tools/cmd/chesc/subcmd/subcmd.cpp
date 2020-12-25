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
    struct CommandOptions {
        static std::string input;
        static std::string settings;
    };


    enum ErrorLogProp {
        Error_UnknownError
    };


    enum WarnLogProp {
        Warn_UnknownWarning
    };


    enum NoteLogProp {
        Note_UnknownNote,
        Note_InvalidCommandArgument
    };


    class ChesSubCommands {
    public:
        static void addAllCommands(Command &cmd) {
            cmd.addProc("cmp", ChesSubCommands::cmd_cmp);
        }

        static void cmd_cmp(PropMap &options, PropMap &settings) {
            try {
                if(!options.exists(CommandOptions::input))
                    Console::log(LogType_Note, Note_InvalidCommandArgument, { { "option", "-" + CommandOptions::input } });

                Compiler cmp;
                std::cout << "cmp" << std::endl;
            } catch(FileError excep) {
                std::cout << "fileerror (" << excep.type << ")" << std::endl;
            }
        }
    };
}
