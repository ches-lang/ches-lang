#pragma once

#include "run.hpp"


ches::cmd::Run::Run() {
    if(g_cmd.existsArgKey("-help")) {
        this->showHelpMessage();
        return;
    }

    if(g_cmd.existsArgKey("-i") && g_cmd.argKeyAt("-i") != "") {
        this->interpretProgram();
        return;
    }

    Console::log(LogType_Error, 5899, {}, true);
}

void ches::cmd::Run::interpretProgram() {
    //Interpreter itp(g_cmd.argKeyAt("-i"), FileManager::readByteSeq(g_cmd.argKeyAt("-i")));
    //itp.runProgram();
}

void ches::cmd::Run::showHelpMessage() {
    Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
}
