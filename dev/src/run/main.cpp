#include <iostream>

#include "interpreter.cpp"


void runCommand(Command cmd) {
    if(cmd.exists("-help")) {
        Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
        return;
    }

    if(cmd.existsKey("-i") && cmd.at("-i") != "") {
        // プログラムを実行
        Interpreter itp(cmd.at("-i"), FileManager::readByteSeq(cmd["-i"]));
        itp.run();
        return;
    }

    Console::log(LogType_Error, 5899, {}, true);
}
