#pragma once

#include "../../../compiler/ches/compiler.cpp"

#include "cmp.hpp"


ches::cmd::Cmp::Cmp() {
    if(g_cmd.existsArgKey("-help")) {
        this->showHelpMessage();
        return;
    }

    if(g_cmd.existsArgKey("-i") && g_cmd.argKeyAt("-i") != "") {
        this->compileProgram();
        return;
    }

    Console::log(LogType_Error, 5899, {}, true);
}

void ches::cmd::Cmp::compileProgram() {
    Compiler cmp;
    cmp.compile();
}

void ches::cmd::Cmp::showHelpMessage() {
    Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
    Console::writeln();
    Console::writeln("<filepath>\t{$HelpMessage_CompileSpecifiedFiles}");
}
