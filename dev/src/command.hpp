#pragma once

#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>



class Command {

private:

    static Options options;

    static void c_ches() {
        if(options.exists("-help")) {
            Console::write("-help\t{$HelpMessage_ShowHelpMessage}");
            Console::write("-ver\t{$HelpMessage_ShowChestnutVersion}");
            Console::write("\n");
            Console::write("cmp\t{$HelpMessage_CompileSpecifiedFiles}");
            Console::write("set\t{$HelpMessage_SeeOrChangeSettings}");
            //std::cout << "-help" << "\t" << "show help message" << std::endl;
            //std::cout << "-ver" << "\t" << "show chestnut version" << std::endl;
            //std::cout << std::endl;
            //std::cout << "cmp" << "\t" << "compile specified file(s)" << std::endl;
            //std::cout << "set" << "\t" << "see or change settings" << std::endl;
        }
    }

    static void c_cmp() {
        if(options.exists("-help")) {
            //std::cout << "-help" << "\t" << "show help message" << std::endl;
            //std::cout << std::endl;
            //std::cout << "<filepath>" << "\t" << "compile a specified file" << std::endl;
            Console::write("-help\t{$HelpMessage_ShowHelpMessage}");
            Console::write("\n");
            Console::write("<filepath>\t{$HelpMessage_CompileSpecifiedFiles}");
            return;
        }

        if(options.exists("-i") && options.get("-i") != "") {
            Compiler cmp(options);
            cmp.compile();
        } else {
            Console::log(LogType_Error, "5899", {}, true);
        }
    }

    static void c_run() {
        if(options.exists("-help")) {
            Console::write("-help\t{$HelpMessage_ShowHelpMessage}");
            //std::cout << "-help" << "\t" << "show help message" << std::endl;
            return;
        }

        if(options.exists("-i") && options.get("-i") != "") {
            Interpreter itp(options);
            itp.run();
        } else {
            Console::log(LogType_Error, "5899", {}, true);
        }
    }

    static void c_set() {
    }

public:

    Command();

    void runCommand(std::string cmd, Options opt);
};

Options Command::options;
