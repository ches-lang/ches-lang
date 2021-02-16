/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


namespace ches::cmd::chesc {
    class ChescCommand : public SubCommands {
    public:
        ChescCommand(int argc, char* argv[], std::string defaultCmdName);

    private:
        void init(std::vector<std::string> args, std::string defaultCmdName) override;

        static void cmd_cmp(Command &cmd) {
            std::cout << "cmp" << std::endl;
        }

        static void cmd_help(Command &cmd) {
            std::cout << "help" << std::endl;
        }

        static void cmd_set(Command &cmd) {
            if(cmd.cmdOptionMap.size() == 0)
                Console::note.print("{^config.setting.note.specifySettingName}", true);

            std::unordered_map<std::string, std::string> outputOptionMap;
            std::unordered_map<std::string, std::string> editedOptionMap;

            for(const auto [ key, value ] : cmd.cmdOptionMap) {
                std::string settingKey = key.substr(1);

                if(!Configuration::settings.exists(settingKey))
                    Console::error.print("{^config.setting.error.unknownSettingName}", { { "{^config.setting.words.settingName}", settingKey } }, true);

                std::string settingValue = Configuration::settings.get(settingKey);

                switch(value.values.size()) {
                    case 0: {
                        outputOptionMap[settingKey] = settingValue;
                    } break;

                    case 1: {
                        std::string newValue = value.values.at(0);

                        ChescCommand::checkSettingPropFormat(settingKey, newValue);

                        outputOptionMap[settingKey] = settingValue + " -> " + newValue;
                        editedOptionMap[settingKey] = newValue;
                    } break;

                    default: {
                        Console::error.print("{^command.tooManyOptionValues}", { { "{^command.words.optionName}", settingKey } }, true);
                    } break;
                }
            }

            if(editedOptionMap.size() != 0) {
                try {
                    Configuration::settings.edit(editedOptionMap);
                } catch(ConfigurationException excep) {
                    Console::error.print("{^config.setting.error.failedToParseSettingData}", { { "{^general.words.errorType}", "ConfigurationException [" + std::to_string(excep.type) + "]" } }, true);
                } catch(FileManagerException excep) {
                    Console::error.print("{^config.setting.error.failedToSaveSettingData}", { { "{^file.words.path}", excep.target }, { "{^general.words.errorType}", "FileManagerException [" + std::to_string(excep.type) + "]" } }, true);
                }
            }

            Console::note.print("{^config.setting.note.settingList}", outputOptionMap);
        }

        static void checkSettingPropFormat(std::string propName, std::string propValue) {
            if(propName == "lang") {
                if(std::regex_search(propValue, std::regex("[^a-zA-Z0-9\\-_]")))
                    Console::error.print("{^config.setting.error.invalidLanguageName}", { { "{^config.setting.words.languageName}", propValue } }, true);

                std::string homeDirPath = Configuration::getEnvironmentVariable(Configuration::homeDirEnvName);
                std::string path = homeDirPath + "/langpack/" + propValue;

                if(!FileManager::exists(path) || !FileManager::isDirectory(path))
                    Console::error.print("{^config.setting.error.settingFilePathNotFound}", { { "{^file.words.path}", path } }, true);

                return;
            }
        }
    };
}
