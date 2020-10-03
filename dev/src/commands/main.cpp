#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <random>
#include <regex>
#include <stack>
#include <string>
#include <unordered_map>
#include <uuid/uuid.h>
#include <vector>

#include "../shared/command.cpp"
#include "../shared/console.cpp"


int main(int argc, char *argv[]) {
    Console::displayCountLimit = 20;
    Console::loadLangPacks("ja", "en");

    Command cmd(argc, argv);
    cmd.run();

    // 終了時のログを出力
    Console::printDebugLog("finish debugger");
}
