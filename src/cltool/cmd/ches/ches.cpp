#pragma once

#include "ches.hpp"


ches::cmd::Ches::Ches() {
    if(g_cmd.existsArgKey("-help")) {
        this->showHelpMessage();
        return;
    }

    ches::Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
}

void ches::cmd::Ches::showHelpMessage() {
    ches::Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
    ches::Console::writeln("-ver\t{$HelpMessage_ShowChestnutVersion}");
    ches::Console::writeln();
    ches::Console::writeln("cmp\t{$HelpMessage_CompileSpecifiedFiles}");
    ches::Console::writeln("set\t{$HelpMessage_SeeOrChangeSettings}");
}
