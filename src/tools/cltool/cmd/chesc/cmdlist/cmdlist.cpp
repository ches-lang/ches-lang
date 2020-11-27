#pragma once


namespace ches {
    class Chesc {
    public:
        static void cmd_default() {
            if(g_cmd.existsArgKey("-help")) {
                Chesc::showHelpMessage();
                return;
            }

            if(g_cmd.existsArgKey("-i") && g_cmd.argKeyAt("-i") != "") {
                Chesc::compileProgram();
                return;
            }

            Console::log(LogType_Error, 5899, {}, true);
        }

        static void compileProgram() {
            Compiler cmp;
            cmp.compile();
        }

        static void showHelpMessage() {
            Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
            Console::writeln();
            Console::writeln("<filepath>\t{$HelpMessage_CompileSpecifiedFiles}");
        }
    };
};
