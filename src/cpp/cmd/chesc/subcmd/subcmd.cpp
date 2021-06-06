/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
 *
 * Copyright © 2019-2021 Garnet3106 All rights reserved.
 */


#pragma once


namespace ches::cmd::chesc {
    class ChescCommand : public SubCommands {
    public:
        ChescCommand(int argc, char* argv[], std::string defaultCmdName) noexcept;

    private:
        void init(std::vector<std::string> args, std::string defaultCmdName) noexcept override;

        static void cmd_cmp(Command &cmd) noexcept {
            std::cout << "cmp" << std::endl;

            std::string inputOptionName = "-i";
            std::string refOptionName = "-ref";
            std::string outputOptionName = "-o";

            std::string chesFileExt = "ches";
            std::string chescFileExt = "chesc";

            // 入力ファイルを取得

            if(cmd.cmdOptionMap.count(inputOptionName) == 0)
                Console::error.print("{^command.error.cmp.noInputFile}", { { "note", "specify an input file with -i option." }, { "note", "show help messages with `chesc help` command" } }, true);

            std::vector<std::string> inputOptionValues = cmd.cmdOptionMap[inputOptionName].values;
            std::vector<std::string> inputFilePaths;

            if(inputOptionValues.size() == 0)
                Console::error.print("{^command.error.cmp.noInputFile}", { { "note", "specify an input file with -i option." } }, true);

            bool inputPathNotExists = false;

            try {
                for(std::string path : inputOptionValues) {
                    if(!FileManager::exists(path)) {
                        Console::error.print("input file not found", { { "path", path } });
                        inputPathNotExists = true;
                    }
                }

                if(inputPathNotExists)
                    Console::terminate();

                inputFilePaths = FileManager::filterFilePathsWithExtensionName(inputOptionValues, chesFileExt);
            } catch(FileManagerException except) {
                Console::error.print("unknown file error", { { "path", except.target } }, true);
            }

            if(inputFilePaths.size() == 0)
                Console::error.print("{^command.error.cmp.noInputFile}", { { "note", "is there any ches file in what you specified?" } }, true);

            // 参照ファイルパスを取得

            std::vector<std::string> refFilePaths;

            if(cmd.cmdOptionMap.count(refOptionName) == 1) {
                std::vector<std::string> refOptionValues = cmd.cmdOptionMap.at(refOptionName).values;

                try {
                    refFilePaths = FileManager::filterFilePathsWithExtensionName(refOptionValues, chescFileExt);
                } catch(FileManagerException except) {
                    switch(except.type) {
                        case FileManagerException_NotFilePath:
                        Console::error.print("reference file not found", { { "path", except.target } }, true);
                        break;
                    }
                }
            }

            // ファイルパスの重複を検知

            // CPEG ファイルパスを取得

            std::string homeDirPath = Configuration::getEnvironmentVariable(Configuration::homeDirEnvName);

            if(homeDirPath == "")
                Console::error.print("environment variable not set", { { "variable name", Configuration::homeDirEnvName } }, true);

            std::string cpegFilePath = homeDirPath + "/cpeg/syntax.cpeg";

            std::cout << "input files" << std::endl;
            for(std::string path : inputFilePaths)
                std::cout << path << std::endl;
            std::cout << std::endl;
            std::cout << "ref files" << std::endl;
            for(std::string path : refFilePaths)
                std::cout << path << std::endl;
            std::cout << std::endl;

            try {
                if(!FileManager::exists(cpegFilePath))
                    Console::error.print("cpeg file not found", { { "path", cpegFilePath } }, true);

                Compiler cmp(inputFilePaths, cpegFilePath);
            } catch(CPEGException except) {
                Console::error.print("failed to load cpeg file", { { "type", std::to_string(except.type) } }, true);
            } catch(FileManagerException except) {
                Console::error.print("unknown file error", { { "path", except.target } }, true);
            }
        }

        static void cmd_help(Command &cmd) noexcept {
            Console::note.print("{^command.words.commandHelp} - {^command.words.subCommandList}", {
                { "cmp", "{^command.help.cmp}" },
                { "help", "{^command.help.help}" },
                { "set", "{^command.help.set}" }
            });
        }

        static void cmd_set(Command &cmd) noexcept {
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

        static void checkSettingPropFormat(std::string propName, std::string propValue) noexcept {
            if(propName == Configuration::langSettingName) {
                try {
                    if(std::regex_search(propValue, std::regex("[^a-zA-Z0-9\\-_]")))
                        Console::error.print("{^config.setting.error.invalidSettingValue}", { { "{^general.words.errorType}", "{^config.setting.error.invalidLanguageName}" },
                            { "{^config.setting.words.settingName}", propName }, { "{^config.setting.words.settingValue}", propValue } }, true);
                } catch(std::regex_error excep) {
                    Console::error.print("{^general.error.internalError}", { { "{^general.words.location}", __func__ } }, true);
                }

                std::string homeDirPath = Configuration::getEnvironmentVariable(Configuration::homeDirEnvName);
                std::string path = homeDirPath + "/langpack/" + propValue;

                if(!FileManager::exists(path) || !FileManager::isDirectory(path))
                    Console::error.print("{^config.setting.error.invalidSettingValue}", { { "{^general.words.errorType}", "{^config.setting.error.settingFilePathNotFound}" },
                        { "{^config.setting.words.settingName}", propName }, { "{^config.setting.words.settingValue}", propValue }, { "{^file.words.path}", path } }, true);

                return;
            }

            if(propName == Console::logLimitSettingName) {
                int logLimit;

                try {
                    Console::logLimitToInt(propValue);
                } catch(ConfigurationException excep) {
                    Console::error.print("{^config.setting.error.invalidSettingValue}", { { "{^config.setting.words.settingName}", propName }, { "{^config.setting.words.settingValue}", propValue } }, true);
                }

                return;
            }
        }
    };
}
