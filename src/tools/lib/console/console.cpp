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
    private:
        static LangPack langPack;
        // value: 2文字の言語コード; デフォルトは英語 ("en")
        static std::string langCode;

        inline static std::string getLogPropName(LogType logType, int logNum) {
            return logTypeNameMap.at(logType) + "_" + std::to_string(logNum);
        }

    public:
        static void log(LogType logType, int logNum, log_detail_map detailMap = {}, bool terminate = false) {
            if(!Console::langPack.loaded)
                Console::langPack.load("./src/tools/cmd/chesc/data/langpack.cset");

            std::string logTypeName = logTypeNameMap.at(logType);
            int logTypeColor = logTypeColorMap.at(logType);

            std::string logPropName = Console::getLogPropName(logType, logNum);
            std::string logMsg = Console::langPack.getValue(logPropName, Console::langCode);

            std::cout << "\033[" << logTypeColor << "m|" << logPropName << "|\033[m " << logMsg << std::endl;
            std::cout << std::endl;

            if(terminate) {
                std::string terminateMsg = Console::langPack.getValue("msg_terminate", Console::langCode);
                std::cout << terminateMsg << std::endl;
                exit(-1);
            }
        }

        // spec: langCodeが不適切な形式であっても値を受け入れる
        static void setLangCode(std::string langCode) {
            Console::langCode = langCode;
        }
    };
}
