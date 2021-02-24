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
    enum ConfigurationExceptionType {
        ConfigurationException_Unknown,
        ConfigurationException_DuplicatedPropName,
        ConfigurationException_InvalidEnvironmentVariable,
        ConfigurationException_InvalidPropName,
        ConfigurationException_InvalidPropValue,
        ConfigurationException_InvalidSettingValue,
        ConfigurationException_InvalidSyntax,
        ConfigurationException_UndefinedSettingProperty,
        ConfigurationException_UnknownPropName
    };


    class ConfigurationException {
    public:
        ConfigurationExceptionType type;
        std::string target = "";

        ConfigurationException() noexcept;

        ConfigurationException(ConfigurationExceptionType type) noexcept;

        ConfigurationException(ConfigurationExceptionType type, std::string target) noexcept;
    };


    class Configuration {
    protected:
        std::string path = "";
        std::unordered_map<std::string, std::string> dataMap;

    public:
        static std::string homeDirEnvName;

        static Configuration settings;
        static Configuration langPack;

        static std::string langSettingName;

        Configuration() noexcept;

        /*
         * arg: editedOptionMap: 設定値を編集したオプションマップ
         * excep: ConfigurationException [InvalidPropValue, UnknownPropName] / FileManager::getLines(std::string) / Configuration::toPropPair(std::string)
         */
        void edit(std::unordered_map<std::string, std::string> editedOptionMap);

        /*
         * ret: プロパティ名が存在するかどうか
         */
        bool exists(std::string key) noexcept;

        /*
         * ret: プロパティ名が見つかれば対応するプロパティ値、見つからなければ propName を返す
         */
        std::string get(std::string key) noexcept;

        /*
         * ret: 環境変数が存在する場合は設定値、存在しない場合は空文字を返す
         */
        inline static std::string getEnvironmentVariable(std::string envName) noexcept {
            const char *value = std::getenv(envName.c_str());

            if(value == NULL)
                return "";

            return value;
        }

        /*
         * excep: ConfigurationException [InvalidEnvironmentVariable, UndefinedSettingProperty] / Configuratioin::loadData(std::string)
         */
        static void loadEachData() {
            try {
                std::string homeDirPath = Configuration::getEnvironmentVariable(Configuration::homeDirEnvName);

                if(homeDirPath == "")
                    throw ConfigurationException(ConfigurationException_InvalidEnvironmentVariable);

                Configuration::settings.loadData(homeDirPath + "/settings/chesc.cnf");

                std::string langSettingValue = Configuration::settings.get(Configuration::langSettingName);

                if(!Configuration::settings.exists(langSettingName) || langSettingValue == "")
                    throw ConfigurationException(ConfigurationException_UndefinedSettingProperty, langSettingName);

                Configuration::langPack.loadData(homeDirPath + "/langpack/" + langSettingValue);
            } catch(FileManagerException excep) {
                throw excep;
            } catch(ConfigurationException excep) {
                throw excep;
            }
        }

        /*
         * excep: ConfigurationException [DuplicatedPropName] / FileManager::getLines(std::string) / toPropPair(std::string)
         */
        void loadData(std::string path);

        void print() noexcept;

        /*
         * note: プロパティ名の重複検査は行われない
         * ret: プロパティ形式の行であればプロパティ名とプロパティ値のペア、そうでなければ null_ptr を返す
         * excep: ConfigurationException [InvalidSyntax, InvalidPropName]
         */
        static std::pair<std::string, std::string> toPropPair(std::string line) {
            if(line.size() == 0)
                return {};

            if(line.at(0) == '#')
                return {};

            int separatorIndex = 0;

            for(; separatorIndex < line.size(); separatorIndex++)
                if(line.at(separatorIndex) == '=')
                    break;

            if(line.size() == separatorIndex)
                throw ConfigurationException(ConfigurationException_InvalidSyntax);

            std::string propName = line.substr(0, separatorIndex);
            std::string propValue = line.substr(separatorIndex + 1);

            Configuration::removeBothSideSpaces(propName);
            Configuration::removeBothSideSpaces(propValue);

            if(propName == "")
                throw ConfigurationException(ConfigurationException_InvalidPropName);

            return std::make_pair(propName, propValue);
        }

    private:
        static void removeBothSideSpaces(std::string &text) noexcept {
            int beforeSpaceLen = 0;
            int behindSpaceLen = 0;

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
