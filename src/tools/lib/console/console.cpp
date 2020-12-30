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
        { LogType_Warn, 35 },
        { LogType_Note, 36 }
    };


    class Console {
    public:
        // note: ログの合計数; デフォルト値は0
        static unsigned int logCount;
        // note: デフォルト値は制限なし (0); 値はコマンド設定による
        static unsigned int logLimit;
        // note: ログ制限時のメッセージのログ番号; デフォルト値は0
        static int logLimitLogNum;

        inline static std::string getLangText(LogType logType, int logNum) {
            std::string propName = logTypeNameMap.at(logType) + "_" + std::to_string(logNum);
            return LangPack::data.get(propName);
        }

        static void log(LogType logType, int logNum, log_detail_map details = {}, bool terminate = false) {
            if(Console::logLimit == 0 || Console::logLimit >= Console::logCount) {
                // proc: ログメッセージを表示
                int logTypeColor = logTypeColorMap.at(logType);
                std::string logTitle = logTypeNameMap.at(logType) + "_" + std::to_string(logNum);
                std::string logMsg = Console::getLangText(logType, logNum);

                std::cout << "\033[" << logTypeColor << "m|" << logTitle << "|\033[m " << logMsg << std::endl;

                for(auto pair : details) {
                    std::string detailName = LangPack::data.get(pair.first);
                    std::string detailValue = pair.second;

                    std::cout << "\t" << detailName << ": " << detailValue << std::endl;
                }

                std::cout << std::endl;
            }

            // note: ログ表示の条件とは異なるので注意
            if(Console::logLimit != 0 && Console::logLimit - 1 == Console::logCount) {
                // note: 無限ループを防止するためログのチェックを入れる
                if(!(logType == LogType_Note && logNum == Console::logLimitLogNum)) {
                    // proc: ログ制限のメッセージを表示
                    Console::log(LogType_Note, Console::logLimitLogNum, { { "log_limit", std::to_string(Console::logLimit) } });
                }
            }

            Console::logCount++;

            if(terminate) {
                std::string terminateMsg = LangPack::data.get("msg_terminate");
                std::cout << terminateMsg << std::endl;
                exit(-1);
            }
        }
    };
}
