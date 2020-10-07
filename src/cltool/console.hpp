#pragma once


namespace ches {
    enum LogType : int {
        LogType_Unknown,
        LogType_Error,
        LogType_Notice,
        LogType_Warning,
    };

    class Console {
    public:

        static std::map<std::string, std::string> langProps;

        static bool debugMode;

        static int displayCount;
        // 0の場合は制限なし
        static int displayCountLimit;

        static bool errored;
        static bool noticed;
        static bool warned;

        static void exitIfDisplayed() {
            if(Console::hasDisplayed())
                exit(-1);
        }

        static std::string getLogCode(int code) {
            std::string logCode = std::to_string(code);

            while(logCode.length() < 4)
                logCode = "0" + logCode;

            return logCode;
        }

        // ログの言語パックのプロパティ名を取得します。
        static std::string getLogPropertyName(int type = LogType_Unknown, int code = 0) {
            std::string logType = "Unknown";

            switch(type) {
                case LogType_Error:
                logType = "Error";
                break;

                case LogType_Notice:
                logType = "Note";
                break;

                case LogType_Warning:
                logType = "Warn";
                break;
            }

            std::string logCode = Console::getLogCode(code);
            return logType + (type != LogType_Unknown ? "_" + logCode : "") + "_Message";
        }

        static std::string getLogType(int type) {
            switch(type) {
                case LogType_Error:
                return "err";

                case LogType_Notice:
                return "note";

                case LogType_Warning:
                return "warn";
            }

            return "unknown";
        }

        // ANSIエスケープの文字色を返します。
        static std::string getLogTypeColor(int type) {
            switch(type) {
                case LogType_Error:
                return "31";

                case LogType_Notice:
                return "36";

                case LogType_Warning:
                return "35";
            }

            // デフォルトでは白色を返す
            return "37";
        }

        static std::string getText(std::string propName) {
            if(Console::langProps.count(propName) == 0)
                return "{$" + propName + "}";

            return Console::langProps[propName];
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

        static void loadLangPack(std::string lang = "en", std::string backupLang = "en") {
            // リリース時にはパスを直してください
            #define LANG_FILE_PATH(langName)    "src/cltool/langpacks/" + langName + ".lang"

            bool existsLangPack = true;

            if(!std::filesystem::exists(LANG_FILE_PATH(lang))) {
                if(!std::filesystem::exists(LANG_FILE_PATH(backupLang))) {
                    Console::log(LogType_Error, 6923, {}, true);
                } else {
                    existsLangPack = false;
                    lang = backupLang;
                }
            }

            std::vector<std::string> lines = Console::readTextLine(LANG_FILE_PATH(lang));

            for(std::string ln : lines) {
                if(ln == "")
                    continue;

                std::string propName = "";
                std::string propVal = "";
                bool reachedDiv = false;

                for(char ch : ln) {
                    if(!reachedDiv) {
                        if(ch == ' ') {
                            reachedDiv = true;
                        } else {
                            propName += ch;
                        }
                    } else {
                        propVal += ch;
                    }
                }

                Console::langProps[propName] = propVal;
            }

            if(!existsLangPack)
                Console::log(LogType_Error, 6923);
        }

        static void log(int type = LogType_Unknown, int code = 0, std::unordered_map<std::string, std::string> details = {}, bool terminate = false) {
            // ログの履歴があれば改行を入れる
            if(Console::hasDisplayed())
                Console::writeln();

            Console::recordLogHistory(type);

            std::string color = Console::getLogTypeColor(type);
            std::string propName = Console::getLogPropertyName(type, code);
            std::string logType = Console::getLogType(type);
            std::string logCode = Console::getLogCode(code);

            Console::writeln("\033[" + color + "m|" + logType + (type != LogType_Unknown ? logCode : "") + "|\033[m {$" + propName + "}");

            for(auto det : details)
                Console::writeln("\t{$LogDetailName_" + det.first + "}: " + det.second + "");

            if(Console::displayCount == Console::displayCountLimit)
                Console::log(LogType_Notice, 7148, { { "Limit", std::to_string(Console::displayCountLimit) } }, true);

            if(type == LogType_Warning && !Console::warned) {
                Console::log(LogType_Notice, 4247, {}, true);
            }

            if(terminate) {
                Console::writeln();
                Console::printDebugLog("end debugger with error");
                exit(-1);
            }
        }

        static void printDebugLog(std::string title) {
            if(Console::debugMode)
                return;

            Console::writeln("\033[1m--- " + title + " ---\033[m");
            Console::writeln();
        }

        static void printDebugLog(std::string title, std::vector<std::string> lines) {
            if(Console::debugMode)
                return;

            Console::printDebugLog(title);

            for(std::string ln : lines)
                Console::writeln("\033[2m  " + ln + "\033[m");

            if(lines.size() > 0)
                Console::writeln();
        }

        static std::vector<std::string> readTextLine(std::string path) {
            try {
                std::ifstream ifs(path);

                if(!ifs.is_open())
                    Console::log(LogType_Error, 327, { { "Path", path } }, true);

                if(ifs.fail())
                    Console::log(LogType_Error, 6845, { { "Path", path } }, true);

                std::vector<std::string> res;
                std::string line;

                while(std::getline(ifs, line)) {
                    if(line == "" || line[0] == '#') continue;
                    else res.push_back(line);
                }

                ifs.close();
                return res;
            } catch(std::exception excep) {
                Console::log(LogType_Error, 6845, { { "Path", path } }, true);
            }

            return {};
        }

        static void write(std::string value) {
            std::string output;
            bool inPropName = false;
            std::string propName = "";

            for(int i = 0; i < value.length(); i++) {
                switch(value[i]) {
                    case '{':
                    if(i < value.length() - 1 && value[i + 1] == '$') {
                        inPropName = true;
                        i++;
                    } else {
                        output += { value[i] };
                    }
                    break;

                    case '}':
                    if(inPropName) {
                        output += Console::getText(propName);
                        propName = "";
                        inPropName = false;
                    } else {
                        output += { value[i] };
                    }
                    break;

                    default:
                    if(inPropName) {
                        propName += { value[i] };
                    } else {
                        output += { value[i] };
                    }
                    break;
                }
            }

            std::cout << output;
        }

        static void writeln() {
            Console::write("\n");
        }

        static void writeln(std::string value) {
            Console::write(value + "\n");
        }
    };

    std::map<std::string, std::string> Console::langProps;

    bool Console::debugMode = false;

    int Console::displayCount = 0;
    int Console::displayCountLimit = 20;

    bool Console::errored = false;
    bool Console::noticed = false;
    bool Console::warned = false;
};
