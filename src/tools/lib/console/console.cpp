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
    typedef std::unordered_map<std::string, std::string>    log_detail_map;


    enum LogType {
        LogType_Error,
        LogType_Warn,
        LogType_Note
    };


    std::unordered_map<LogType, std::string> logTypeNameMap {
        { LogType_Error, "error" },
        { LogType_Warn, "warn" },
        { LogType_Note, "note" }
    };


    std::unordered_map<LogType, int> logTypeColorMap {
        { LogType_Error, 31 },
        { LogType_Warn, 36 },
        { LogType_Note, 35 }
    };


    class Console {
    public:
        static PropMap langPack;
        static std::string langPackFilePath;

        // note: 2文字の言語コード; デフォルト値は英語 ("en")
        static std::string langCode;
        // note: デフォルト値は制限なし (0); 値はコマンド設定による
        static unsigned int logLimit;

        inline static std::string getLangText(std::string propName) {
            std::string propKey = propName + "_" + Console::langCode;
            return Console::langPack.get(propKey);
        }

        inline static std::string getLangText(LogType logType, int logNum) {
            std::string propName = logTypeNameMap.at(logType) + "_" + std::to_string(logNum);
            return Console::getLangText(propName);
        }

        static void log(LogType logType, int logNum, log_detail_map detailMap = {}, bool terminate = false) {
            if(!Console::langPack.loaded)
                Console::langPack.load(Console::langPackFilePath);

            int logTypeColor = logTypeColorMap.at(logType);
            std::string logTitle = logTypeNameMap.at(logType) + "_" + std::to_string(logNum);
            std::string logMsg = Console::getLangText(logType, logNum);

            std::cout << "\033[" << logTypeColor << "m|" << logTitle << "|\033[m " << logMsg << std::endl;

            for(auto detailPair : detailMap) {
                std::string detailName = Console::getLangText(detailPair.first);
                std::string detailValue = detailPair.second;

                std::cout << "\t" << detailName << ": " << detailValue << std::endl;
            }

            std::cout << std::endl;

            if(terminate) {
                std::string terminateMsg = Console::getLangText("msg_terminate");
                std::cout << terminateMsg << std::endl;
                exit(-1);
            }
        }
    };
}
