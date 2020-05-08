#pragma once

#include <iostream>
#include <string>
#include <unordered_map>



enum LogType : int {
    LogType_Error,
    LogType_Notice,
    LogType_Warning
};



class Console {

public:

    static std::map<std::string, std::string> langPackProperties;

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
        std::string output;
        bool inPropName = false;
        std::string propName = "";

        for(int i = 0; i < str.length(); i++) {
            switch(str[i]) {
                case '{':
                if(i < str.length() - 1 && str[i + 1] == '$') {
                    std::cout<<"start"<<std::endl;
                    inPropName = true;
                    i++;
                } else {
                    std::cout<<"exit"<<std::endl;
                    output += std::to_string(str[i]);
                }
                break;

                case '}':
                if(inPropName) {
                    std::cout<<"end"<<std::endl;
                    output += Console::getText(propName);
                    propName = "";
                    inPropName = false;
                } else {
                    std::cout<<"noend"<<std::endl;
                    output += std::to_string(str[i]);
                }
                break;

                default:
                if(inPropName) {
                    propName += std::to_string(str[i]);
                } else {
                    output += std::to_string(str[i]);
                }
                break;
            }
        }

        std::cout << output;
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

    static std::string getText(std::string propName) {
        if(Console::langPackProperties.count(propName) == 0)
            return propName;

        return Console::langPackProperties[propName];
    }

    static void loadLangPacks(std::string lang = "en") {
        // リリース時にはパスを直してください
        std::string filePath = "./chestnut/langpacks/" + lang + ".lang";

        if(!std::filesystem::exists(filePath)) {
            Console::log(LogType_Notice, "6923");
            lang = "en";
        }

        std::vector<std::string> lines = Console::readTextLine(filePath);

        for(std::string ln : lines) {
            if(ln == "") continue;

            std::string propName = "";
            std::string propVal = "";
            bool reachedDiv = false;

            for(char ch : ln) {
                if(!reachedDiv) {
                    if(ch == ' ') reachedDiv = true;
                    else propName += ch;
                } else {
                    propVal += ch;
                }
            }

            Console::langPackProperties[propName] = propVal;
        }
    }

    static std::vector<std::string> readTextLine(std::string path) {
        try {
            std::ifstream ifs(path);

            if(!ifs.is_open())
                Console::log(LogType_Error, "0327", { { "Path", path } }, true);

            if(ifs.fail())
                Console::log(LogType_Error, "6845", { { "Path", path } }, true);

            std::vector<std::string> res;
            std::string line;

            while(std::getline(ifs, line))
                res.push_back(line);

            ifs.close();
            return res;
        } catch(std::exception excep) {
            Console::log(LogType_Error, "6845", { { "Path", path } }, true);
        }

        return {};
    }
};



std::map<std::string, std::string> Console::langPackProperties;

int Console::displayCount;
int Console::displayCountLimit;

bool Console::errored;
bool Console::noticed;
bool Console::warned;
