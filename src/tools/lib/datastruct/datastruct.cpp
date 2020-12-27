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
    typedef unsigned char   byte;


    std::string trim_string_spaces(std::string string) {
        while(string.size() > 0 && string.at(0) == ' ')
            string = string.substr(1);

        while(string.size() > 0 && string.at(string.size() - 1) == ' ')
            string = string.substr(0, string.size() - 1);

        return string;
    }


    // note: 通常のmapと要素の順序管理が異なる (unordered_mapを使用するため)
    struct PropMap : public std::unordered_map<std::string, std::string> {
        bool loaded = false;

        inline std::string get(std::string propKey) {
            if(!this->exists(propKey))
                return "[" + propKey + "]";

            return this->at(propKey);
        }

        inline bool exists(std::string key) {
            return this->count(key) == 1;
        }

        void load(std::string filePath);
    };
}
