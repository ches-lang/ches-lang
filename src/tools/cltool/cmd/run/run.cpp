#pragma once

#include "../../../interpreter/interpreter.cpp"

#include "run.hpp"


ches::run::Run::Run() {
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

void ches::run::Run::interpretProgram() {
    ByteSeq fileCont;
    FileManager::readByteSeq(g_cmd.argKeyAt("-i"), fileCont);
    Interpreter itp(g_cmd.argKeyAt("-i"), fileCont);
    //itp.runProgram();
}

void ches::run::Run::showHelpMessage() {
    Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
}
