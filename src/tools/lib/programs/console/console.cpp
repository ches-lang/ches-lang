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
    enum LogType {
        LogType_Error,
        LogType_Warn,
        LogType_Note
    };


    class Console {
    public:
        static LangPack langPack;

        inline static void log(std::string message) {
            std::cout << message;
        }

        inline static void logln(std::string message) {
            Console::log(message + "\n");
        }

        inline static void log(LogType type, int logNum, log_prop_map detailMap) {
            std::string logMsg = ;

            Console::logln();
        }
    };
}
