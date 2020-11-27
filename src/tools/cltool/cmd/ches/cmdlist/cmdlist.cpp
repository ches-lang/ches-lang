#pragma once


namespace ches {
    class Ches {
    public:
        static void cmd_default() {
            if(g_cmd.existsArgKey("-help")) {
                Ches::showHelpMessage();
                return;
            }

            if(g_cmd.existsArgKey("-i") && g_cmd.argKeyAt("-i") != "") {
                Ches::interpretProgram();
                return;
            }

            Console::log(LogType_Error, 5899, {}, true);
        }

        static void interpretProgram() {
            // ByteVec fileCont;
            // FileManager::readByteSeq(g_cmd.argKeyAt("-i"), fileCont);
            // Interpreter itp(g_cmd.argKeyAt("-i"), fileCont);
            Interpreter itp(g_cmd.argKeyAt("-i"));
            itp.runProgram();
        }

        static void showHelpMessage() {
            Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
        }
    };
};
