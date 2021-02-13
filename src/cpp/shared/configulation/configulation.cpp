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
    enum ConfigulationExceptionType {
        ConfigulationException_Unknown,
        ConfigulationException_DuplicatedPropName,
        ConfigulationException_InvalidPropName,
        ConfigulationException_InvalidSyntax
    };


    class ConfigulationException {
    public:
        ConfigulationExceptionType type;
        std::string target = "";

        ConfigulationException();

        ConfigulationException(ConfigulationExceptionType type);

        ConfigulationException(ConfigulationExceptionType type, std::string target);
    };


    class Configulation {
    protected:
        std::string path = "";
        std::unordered_map<std::string, std::string> dataMap;

    public:
        static std::string homeDirEnvName;
        static std::string homeDirPath;

        static Configulation settings;
        static Configulation langPack;

        Configulation();

        /*
         * arg: path: configデータのホームディレクトリから見た相対的なディレクトリパスまたはファイルパス
         */
        Configulation(std::string relPath);

        /*
         * ret: プロパティ名が存在するかどうか
         */
        bool exists(std::string key);

        /*
         * ret: プロパティ名が見つかれば対応するプロパティ値、見つからなければ propName を返す
         */
        std::string get(std::string key);

        /*
         * ret: 環境変数が存在する場合は設定値、存在しない場合は空文字を返す
         */
        inline static std::string getEnvironmentVariable(std::string envName) {
            const char *value = std::getenv(envName.c_str());

            if(value == NULL)
                return "";

            return value;
        }

        void print();

    protected:
        void loadData();

    private:
        void removeBothSideSpaces(std::string &text) {
            int beforeSpaceLen, behindSpaceLen = 0;

            for(; beforeSpaceLen < text.size(); beforeSpaceLen++)
                if(text.at(beforeSpaceLen) != ' ')
                    break;

            for(; behindSpaceLen < text.size(); behindSpaceLen++)
                if(text.at(text.size() - behindSpaceLen - 1) != ' ')
                    break;

            if(beforeSpaceLen != 0 && beforeSpaceLen == behindSpaceLen) {
                text = "";
                return;
            }

            text = text.substr(beforeSpaceLen, text.size() - behindSpaceLen - beforeSpaceLen);
        }
    };
}

