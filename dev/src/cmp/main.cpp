#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <regex>
#include <stack>
#include <unordered_map>
#include <vector>

#include "../shared/console.cpp"
#include "../shared/command.cpp"
#include "../shared/filemanager.cpp"
#include "../shared/utility.cpp"
#include "parser.cpp"
#include "lexer.cpp"

#include "compiler.cpp"


void runCommand(Command cmd) {
    if(cmd.existsKey("-help")) {
        Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
        Console::writeln();
        Console::writeln("<filepath>\t{$HelpMessage_CompileSpecifiedFiles}");
        return;
    }

    if(cmd.existsKey("-i") && cmd.at("-i") != "") {
        Compiler cmp;
        cmp.compile(cmd.at("-i"));
        return;
    }

    Console::log(LogType_Error, 5899, {}, true);
}
