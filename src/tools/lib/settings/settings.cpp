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
    typedef std::unordered_map<std::string, std::string>  setting_prop_map;


    class Settings {
    public:
        setting_prop_map propMap;
        bool loaded = false;

        Settings();

        inline bool existsKey(std::string propKey) {
            return this->propMap.count(propKey) == 1;
        }

        inline std::string getValue(std::string propKey) {
            if(!this->existsKey(propKey))
                return "[" + propKey + "]";

            return this->propMap.at(propKey);
        }

        void load(std::string filePath);

    private:
        static std::string trimSpaces(std::string string) {
            while(string.size() > 0 && string.at(0) == ' ')
                string = string.substr(1);

            while(string.size() > 0 && string.at(string.size() - 1) == ' ')
                string = string.substr(0, string.size() - 1);

            return string;
        }
    };


    class LangPack : public Settings {
    public:
        // arg: propNameに言語コードを含めないこと
        inline bool existsKey(std::string propName, std::string langCode) {
            std::string propKey = propName + "_" + langCode;
            return Settings::existsKey(propKey);
        }

        // spec: キーが見つからない場合は角括弧で囲んだpropKeyを返す
        // arg: propNameに言語コードを含めないこと
        inline std::string getValue(std::string propName, std::string langCode) {
            std::string propKey = propName + "_" + langCode;
            return Settings::getValue(propKey);
        }
    };
}
