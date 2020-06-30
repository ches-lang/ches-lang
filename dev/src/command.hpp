#pragma once



class Command {

private:

    static void c_ches() {
        if(g_cmd_data.exists("-help")) {
            Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
            Console::writeln("-ver\t{$HelpMessage_ShowChestnutVersion}");
            Console::writeln();
            Console::writeln("cmp\t{$HelpMessage_CompileSpecifiedFiles}");
            Console::writeln("set\t{$HelpMessage_SeeOrChangeSettings}");
            return;
        }

        Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
    }

    static void c_cmp() {
        if(g_cmd_data.exists("-help")) {
            Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
            Console::writeln();
            Console::writeln("<filepath>\t{$HelpMessage_CompileSpecifiedFiles}");
            return;
        }

        if(g_cmd_data.exists("-i") && g_cmd_data["-i"] != "") {
            Compiler cmp;
            cmp.compile(g_cmd_data["-i"]);
            return;
        }

        Console::log(LogType_Error, 5899, {}, true);
    }

    static void c_run() {
        if(g_cmd_data.exists("-help")) {
            Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
            return;
        }

        if(g_cmd_data.exists("-i") && g_cmd_data["-i"] != "") {
            // プログラムを実行
            Interpreter itp(g_cmd_data["-i"], FileManager::readByteSeq(g_cmd_data["-i"]));
            itp.runProgram();
            return;
        }

        Console::log(LogType_Error, 5899, {}, true);
    }

    static void c_set() {
    }

public:

    Command();

    void runCommand();
};
