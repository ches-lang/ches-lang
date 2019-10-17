#include <iostream>
#include <string>
#include <vector>

#define length(array) (sizeof(array) / sizeof(array[0]))



void error(std::string msg) {
    std::cout << "\033[31m" << "error: " << msg << "\033[m" << std::endl;
}

void command(std::string cmd) {
    std::string cmdList[] = { "comp", "help" };

    bool existsCmd = false;
    for(int i = 0; i < length(cmdList); i++)
        if(cmd == cmdList[i])
            existsCmd = true;

    if(!existsCmd) {
        error("command '" + cmd + "' is not exits");
    }
}

int main(int argc, char *argv[]) {
    std::string cmd;
    if(argc <= 1) cmd = "help";
    else cmd = argv[1];
    command(cmd);
}
