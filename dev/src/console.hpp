#pragma once



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

    static void exitIfDisplayed() {
        if(Console::hasDisplayed())
            exit(-1);
    }

    static void log(int type = LogType_Error, std::string code = "0000", std::unordered_map<std::string, std::string> details = {}, bool terminate = false) {
        // ログの履歴があれば改行を入れる
        if(Console::hasDisplayed())
            Console::writeln();

        Console::recordLogHistory(type);

        std::string color = Console::getLogTypeColor(type);
        std::string msgName = Console::getLogMessageName(type, code);
        std::string prefix = Console::getLogCodePrefix(type);

        //std::cout << "\033[31m" << "|" << prefix << code << "|" << "\033[m" << " " << msg << std::endl;
        Console::writeln("\033[" + color + "m|" + prefix + code + "|\033[m {$" + msgName + "}");

        for(auto dtl : details)
            //std::cout << "\t" << dtl.first << ": " << dtl.second << std::endl;
            Console::writeln("\t{$LogDetailName_" + dtl.first + "}: " + dtl.second + "");

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
                    inPropName = true;
                    i++;
                } else {
                    output += { str[i] };
                }
                break;

                case '}':
                if(inPropName) {
                    output += Console::getText(propName);
                    propName = "";
                    inPropName = false;
                } else {
                    output += { str[i] };
                }
                break;

                default:
                if(inPropName) {
                    propName += { str[i] };
                } else {
                    output += { str[i] };
                }
                break;
            }
        }

        std::cout << output;
    }

    static void writeln() {
        Console::write("\n");
    }

    static void writeln(std::string str) {
        Console::write(str + "\n");
    }

    static std::string getLogTypeColor(int type) {
        switch(type) {
            case LogType_Error:
            return "31";

            case LogType_Notice:
            return "36";

            case LogType_Warning:
            return "35";
        }

        // デフォルトでは黒色を返す
        return "30";
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
            return "{$" + propName + "}";

        return Console::langPackProperties[propName];
    }

    static void loadLangPacks(std::string lang = "en", std::string backupLang = "en") {
        // リリース時にはパスを直してください
        #define LANG_FILE_PATH(path)    "./chestnut/langpacks/" + path + ".lang"

        bool existsLangPack = true;

        if(!std::filesystem::exists(LANG_FILE_PATH(lang))) {
            if(!std::filesystem::exists(LANG_FILE_PATH(backupLang))) {
                Console::log(LogType_Notice, "6923", {}, true);
            } else {
                existsLangPack = false;
                lang = backupLang;
            }
        }

        std::vector<std::string> lines = Console::readTextLine(LANG_FILE_PATH(lang));

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

        if(!existsLangPack)
            Console::log(LogType_Notice, "6923");
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

            while(std::getline(ifs, line)) {
                if(line == "" || line[0] == '#') continue;
                else res.push_back(line);
            }

            ifs.close();
            return res;
        } catch(std::exception excep) {
            Console::log(LogType_Error, "6845", { { "Path", path } }, true);
        }

        return {};
    }

    static void printDebugLog(std::string title) {
        Console::writeln();
        Console::writeln("--- " + title + " ---");
    }

    static void printDebugLog(std::string title, std::vector<std::string> lines) {
        Console::printDebugLog(title);

        if(lines.size() > 0)
            Console::writeln();

        for(std::string ln : lines)
            Console::writeln(ln);
    }
};



std::map<std::string, std::string> Console::langPackProperties;

int Console::displayCount;
int Console::displayCountLimit;

bool Console::errored;
bool Console::noticed;
bool Console::warned;
