/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


namespace ches::shared {
    class Console {
    private:
        static bool logLimitLoaded;

    public:
        static int logLimit;
        static std::string logLimitSettingName;

        static int logCount;

        static Console debug;
        static Console error;
        static Console note;
        static Console warn;

        std::string typeName = "";
        int typeColor = 30;

        Console();

        /*
         * arg: typeColor: ANSI Color Code
         */
        Console(std::string typeName, int typeColor = 30);

        /*
         * excep: ConfigurationException [InvalidSettingValue]
         */
        static int logLimitToInt(std::string value) {
            int logLimit;

            try {
                if(!std::regex_match(value, std::regex("\\-?[0-9]+")))
                    throw nullptr;

                logLimit = std::stoi(value);

                if(logLimit < -1)
                    throw nullptr;
            } catch(...) {
                throw ConfigurationException(ConfigurationException_InvalidSettingValue);
            }

            return logLimit;
        }

        /*
         * excep: Console::logLimitToInt(std::string) と同様
         */
        static int getLogLimit() {
            std::string logLimitSettingValue = Configuration::settings.get(Console::logLimitSettingName);

            if(!Configuration::settings.exists(logLimitSettingName))
                return -1;

            int logLimit;

            try {
                logLimit = Console::logLimitToInt(logLimitSettingValue);
            } catch(ConfigurationException excep) {
                throw excep;
            }

            return logLimit;
        }

        void print(std::string title, bool terminateProc);

        void print(std::string title, std::unordered_map<std::string, std::string> detailMap = {}, bool terminateProc = false);

        static void translateText(std::string &text) {
            try {
                int beginIndex = 0;
                std::regex searchPattern("\\{\\^[a-zA-Z0-9\\._-]+\\}");

                while(beginIndex < text.size()) {
                    std::string tarText = text.substr(beginIndex);
                    std::smatch searchResults;

                    std::regex_search(tarText, searchResults, searchPattern);

                    if(searchResults.size() != 0) {
                        int pos = searchResults.position(0);
                        int len = searchResults.length(0);

                        std::string matched = searchResults.str(0);
                        std::string propName = matched.substr(2, matched.size() - 3);

                        if(Configuration::langPack.exists(propName)) {
                            std::string propValue = Configuration::langPack.get(propName);
                            text.replace(beginIndex + pos, len, propValue);

                            beginIndex += pos + propValue.size();
                        } else {
                            beginIndex += pos + len;
                        }
                    } else {
                        break;
                    }
                }
            } catch(std::regex_error excep) {
                std::cout << "error (std::regex_error) on Console::translateText(std::string&)" << std::endl;
            }
        }
    };
}
