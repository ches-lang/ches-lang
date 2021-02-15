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
                Console::note.print("{^chesc.note.specifySettingName}", true);

            std::unordered_map<std::string, std::string> outputOptionMap;
            std::unordered_map<std::string, std::string> editedOptionMap;

            for(const auto [ key, value ] : cmd.cmdOptionMap) {
                std::string settingKey = key.substr(1);

                if(!Configulation::settings.exists(settingKey))
                    Console::error.print("{^chesc.error.unknownSettingName}", { { "{^chesc.word.settingName}", settingKey } }, true);

                std::string settingValue = Configulation::settings.get(settingKey);

                switch(value.values.size()) {
                    case 0: {
                        outputOptionMap[settingKey] = settingValue;
                    } break;

                    case 1: {
                        std::string newValue = value.values.at(0);
                        outputOptionMap[settingKey] = settingValue + " -> " + newValue;
                        editedOptionMap[settingKey] = newValue;
                    } break;

                    default: {
                        Console::error.print("{^chesc.error.tooManyOptionValues}", { { "{^chesc.word.optionName}", settingKey } }, true);
                    } break;
                }
            }

            if(editedOptionMap.size() != 0) {
                try {
                    Configulation::settings.edit(editedOptionMap);
                } catch(ConfigulationException excep) {
                    std::string excepTypeName;

                    switch(excep.type) {
                        case ConfigulationException_InvalidPropName:
                        excepTypeName = "invalid prop name";
                        break;

                        case ConfigulationException_InvalidPropValue:
                        excepTypeName = "invalid prop value";
                        break;

                        case ConfigulationException_InvalidSyntax:
                        excepTypeName = "invalid syntax";
                        break;

                        case ConfigulationException_UnknownPropName:
                        excepTypeName = "unknown prop name";
                        break;

                        default:
                        excepTypeName = "unknown error";
                        break;
                    }

                    Console::error.print("{^setting.error.failedToParseSettingData}", { { "{^error.word.errorType}", excepTypeName } }, true);
                } catch(FileManagerException excep) {
                    switch(excep.type) {
                        case FileManagerException_NotFilePath:
                        case FileManagerException_PathNotFound:
                        case FileManagerException_FileUnopenable:
                        Console::error.print("{^setting.error.failedToSaveSettingFile}", { { "{^file.word.path}", excep.target } }, true);
                        break;

                        default:
                        Console::error.print("{^file.error.unknownFileError}", { { "file.word.path", excep.target } }, true);
                        break;
                    }
                }
            }

            Console::note.print("{^chesc.note.settingList}", outputOptionMap);
        }
    };
}
