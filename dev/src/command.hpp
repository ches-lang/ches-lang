#pragma once



class Command {

private:

    static Options options;

    static void c_ches() {
        if(options.exists("-help")) {
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
        if(options.exists("-help")) {
            Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
            Console::writeln();
            Console::writeln("<filepath>\t{$HelpMessage_CompileSpecifiedFiles}");
            return;
        }

        if(options.exists("-i") && options["-i"] != "") {
            Compiler cmp(options);
            cmp.compile();
            return;
        }

        Console::log(LogType_Error, "5899", {}, true);
    }

    static void c_run() {
        if(options.exists("-help")) {
            Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
            return;
        }

        if(options.exists("-i") && options["-i"] != "") {
            Interpreter itp(options, FileManager::readByteSeq(options["-i"]));
            itp.run();
            return;
        }

        Console::log(LogType_Error, "5899", {}, true);
    }

    static void c_set() {
    }

public:

    Command();

    void runCommand(std::string cmd, Options opt);
};

Options Command::options;
