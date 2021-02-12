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
        std::string absDirPath = "";
        std::unordered_map<std::string, std::string> langDataMap;

    public:
        static LangPack pack;

        /*
         * arg: absDirPath: 言語パックディレクトリの絶対パス
         */
        LangPack(std::string absDirPath);

        /*
         * ret: プロパティ名が存在するかどうか
         */
        bool exists(std::string propName);

        /*
         * ret: プロパティ名が見つかれば対応するプロパティ値、見つからなければ propName を返す
         */
        std::string get(std::string propName);

        void print();

    private:
        void loadLangPack();

        void removeBothSideSpaces(std::string &text);
    };
}
