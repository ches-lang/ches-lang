#include <iostream>

#include "compiler.cpp"


void runCommand(Command cmd) {
    if(cmd.existsKey("-help")) {
        Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
        Console::writeln("-ver\t{$HelpMessage_ShowChestnutVersion}");
        Console::writeln();
        Console::writeln("cmp\t{$HelpMessage_CompileSpecifiedFiles}");
        Console::writeln("set\t{$HelpMessage_SeeOrChangeSettings}");
        return;
    }

    Console::writeln("-help\t{$HelpMessage_ShowHelpMessage}");
}
