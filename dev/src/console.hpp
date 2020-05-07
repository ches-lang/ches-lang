#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
//#include "language.cpp"

class Language{public:static void loadLangPacks(std::string lang){}};

enum LogType : int {
    LogType_Error,
    LogType_Notice,
    LogType_Warning
};



class Console {

public:

    static int displayCount;
    static int displayCountLimit;   // No limit: 0

    static bool errored;
    static bool noticed;
    static bool warned;

    static void log(int type = LogType_Error, std::string code = "0000", std::unordered_map<std::string, std::string> details = {}, bool terminate = false) {
        // ログの履歴があれば改行を入れる
        if(Console::hasDisplayed())
            Console::write("\n");

        Console::recordLogHistory(type);

        std::string msgName = getLogMessageName(type, code);
        std::string prefix = getLogCodePrefix(type);

        //std::cout << "\033[31m" << "|" << prefix << code << "|" << "\033[m" << " " << msg << std::endl;
        Console::write("\033[31m|" + prefix + code + "|\033[m {$" + msgName + "}\n");

        for(auto dtl : details)
            //std::cout << "\t" << dtl.first << ": " << dtl.second << std::endl;
            Console::write("\t{$LogDetailName_" + dtl.first + "}: " + dtl.second + "\n");

        if(Console::displayCount == Console::displayCountLimit)
            Console::log(LogType_Notice, "7148", { { "Limit", std::to_string(Console::displayCountLimit) } }, true);

        if(type == LogType_Warning && !Console::warned) {
            Console::log(LogType_Notice, "4247", {}, true);
        }

        if(terminate) exit(-1);
    }

    static void write(std::string str) {
        std::cout << str;
    }

    static std::string getLogMessageName(int type = LogType_Error, std::string code = "0000") {
        std::string logType = "CError";

        switch(type) {
            case LogType_Error:
            logType = "CError";

            case LogType_Notice:
            logType = "CNote";

            case LogType_Warning:
            logType = "CWarn";
        }

        return "CError_" + code + "_Message";
    }

    static std::string getLogCodePrefix(int type) {
        switch(type) {
        case LogType_Error:
            return "cerr";

        case LogType_Notice:
            return "cnote";

        case LogType_Warning:
            return "cwarn";
        }

        return "cerr";
    }

    static bool hasDisplayed() {
        return Console::displayCount > 0;
    }

    static void recordLogHistory(int type) {
        Console::displayCount++;

        switch(type) {
            case LogType_Error:
            Console::errored = true;
            break;

            case LogType_Notice:
            Console::noticed = true;
            break;

            case LogType_Warning:
            Console::warned = true;
            break;
        }
    }
};



int Console::displayCount;
int Console::displayCountLimit;

bool Console::errored;
bool Console::noticed;
bool Console::warned;
