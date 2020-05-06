#pragma once

#include <filemanager.hpp>
#include <iostream>
#include <map>
#include <string>
#include "console.hpp"



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

        if(!FileManager::exists()) {
            Console::log(LogType_Notice, "", {});
            lang = "en";
        }

        std::vector<std::string> lines = FileManager::readTextLine(filePath);

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
};

std::map<std::string, std::string> Language::properties;
