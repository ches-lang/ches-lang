/* 
 * 
 * Chestnut 0.0.0
 * 
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 * 
 * Copyright © 2020-2021 Garnet3106 All rights reserved. 
 * 
 */


#pragma once


extern std::string operator+(std::string value1, int value2) {
    return value1 + std::to_string(value2);
}


namespace ches::shared {
    class Console {
    public:
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

        void print(int title, std::unordered_map<int, std::string> detailMap = {});
    };
}
