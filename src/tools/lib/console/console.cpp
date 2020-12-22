/* 
 * 
 * Chestnut 0.0.0
 * 
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 * 
 * Copyright © 2020 Garnet3106 All rights reserved. 
 * 
 */


#pragma once


namespace ches {
    typedef std::unordered_map<int, std::string>  log_detail_map;


    enum LogType {
        LogType_Error,
        LogType_Warn,
        LogType_Note
    };


    std::unordered_map<LogType, std::string> logTypeMap {
        { LogType_Error, "error" },
        { LogType_Warn, "warn" },
        { LogType_Note, "note" }
    };


    class Console {
    public:
        static Settings langPack;
        // value: 2文字の言語コード; デフォルトは英語 ("en")
        static std::string langCode;

        inline static std::string getLogPropKey(LogType logType, int logNum) {
            return logTypeMap.at(logType) + "_" + std::to_string(logNum) + "_" + "Console::langCode";
        }

        static void log(LogType logType, int logNum, log_detail_map detailMap) {
            if(!Console::langPack.loaded)
                Console::langPack.load("./src/tools/cmd/chesc/data/langpack.cset");

            std::string logPropKey = Console::getLogPropKey(logType, logNum);
            std::cout << logPropKey << std::endl;
            std::string logMsg = Console::langPack.getValue(logPropKey);
            std::cout << logMsg << std::endl;
        }
    };
}
