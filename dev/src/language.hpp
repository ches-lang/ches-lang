#pragma once

#include <iostream>
#include <map>
#include <string>
//#include "console.cpp"
//#include "filemanager.cpp"



class Language {

public:

    static std::map<std::string, std::string> properties;

    static std::string getText(std::string propName) {
        if(Language::properties.count(propName) == 0)
            return propName;

        return Language::properties[propName];
    }

    static void loadLangPacks(std::string lang = "en") {
        std::string filePath = "./langpacks/" + lang + ".lang";

        if(!std::filesystem::exists(filePath)) {
            Console::log(LogType_Notice, "6923");
            lang = "en";
        }

        std::vector<std::string> lines = Language::readTextLine(filePath);

        for(std::string ln : lines) {
            if(ln == "") continue;

            std::string propName = "";
            std::string propVal = "";
            bool reachedDiv = false;

            for(char ch : ln) {
                if(!reachedDiv) {
                    if(ch == ' ') reachedDiv = true;
                    else propName += ch;
                } else {
                    propVal += ch;
                }
            }

            Language::properties[propName] = propVal;
        }
    }

    static std::vector<std::string> readTextLine(std::string path) {
        try {
            std::ifstream ifs(path);

            if(!ifs.is_open())
                Console::log(LogType_Error, "0327", { { "Path", path } }, true);

            if(ifs.fail())
                Console::log(LogType_Error, "6845", { { "Path", path } }, true);

            std::vector<std::string> res;
            std::string line;

            while(std::getline(ifs, line))
                res.push_back(line);

            ifs.close();
            return res;
        } catch(std::exception excep) {
            Console::log(LogType_Error, "6845", { { "Path", path } }, true);
        }

        return {};
    }
};
