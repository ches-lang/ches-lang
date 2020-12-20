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
    typedef std::unordered_map<std::string, std::string>  lang_pack_prop_map;


    enum LangPackErrorType {
        LangPackError_DuplicatedPropKey,
        LangPackError_FileLoadFailed,
        LangPackError_InvalidNotation
    };


    class LangPackError {
    public:
        LangPackErrorType type;

        LangPackError();

        LangPackError(LangPackErrorType type);
    };


    class LangPack {
    public:
        lang_pack_prop_map propMap;

        LangPack();

        bool existsKey(std::string propKey);

        std::string getValue(std::string propKey);

        void load(std::string filePath);

    public:
        static std::string trimSpaces(std::string string) {
            while(string.size() > 0 && string.at(0) == ' ')
                string = string.substr(1);

            while(string.size() > 0 && string.at(string.size() - 1) == ' ')
                string = string.substr(0, string.size() - 1);

            return string;
        }
    };
}
