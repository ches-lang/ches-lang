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
        LangPackException_CouldNotOpenLangFile,
        LangPackException_DuplicatedPropName,
        LangPackException_InvalidPropName,
        LangPackException_InvalidSyntax
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
        std::string absFilePath = "";
        std::unordered_map<std::string, std::string> langDataMap;

    public:
        /*
         * arg: absFilePath: 言語ファイルの絶対パス
         */
        LangPack(std::string absFilePath);

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

        void removeBothSideSpaces(std::string &str);
    };
}
