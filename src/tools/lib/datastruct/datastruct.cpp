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
    private:
        // note: 値の改変防止のためprivateに設定; isLoaded()を使うこと
        bool loaded = false;

    public:
        inline bool exists(std::string key) {
            return this->count(key) == 1;
        }

        inline std::string get(std::string key) {
            if(!this->exists(key))
                return "[" + key + "]";

            return this->at(key);
        }

        inline bool isLooaded() {
            return this->loaded;
        }

        void load(std::string filePath);
    };


    struct LangPack : public PropMap {
    public:
        static LangPack data;

        // note: 2文字の言語コード; デフォルト値は"en" (英語)
        static std::string langCode;

        // arg: propName: 言語コードなしのプロップ名
        inline std::string get(std::string propName) {
            std::string key = propName + "_" + LangPack::langCode;

            if(!this->exists(key))
                return "[" + key + "]";

            return this->at(key);
        }
    };


    struct Settings : public PropMap {
    public:
        static Settings data;
    };
}
