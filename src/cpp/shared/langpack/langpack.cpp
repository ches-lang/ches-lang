/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


namespace ches::shared {
    enum LangPackExceptionType {
        LangPackException_Unknown,
        LangPackException_CouldNotOpenLangFile
    };


    class LangPackException {
    public:
        LangPackExceptionType type;
        std::string target = "";

        LangPackException();

        LangPackException(LangPackExceptionType type);

        LangPackException(LangPackExceptionType type, std::string target);
    };


    class LangPack {
    private:
        std::string filePath = "";
        std::unordered_map<std::string, std::string> langDataMap;

    public:
        LangPack(std::string filePath);

        /*
         * ret: プロパティ名が見つからない場合は propName を返す
         */
        std::string getPropValue(std::string propName);

        void print();

    private:
        /*
         * except: LangPackException [CouldNotOpenLangFile]
         */
        void loadLangPack();
    };
}
