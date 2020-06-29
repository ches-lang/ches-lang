#pragma once



struct CommandData : std::unordered_map<std::string, std::string> {

    std::string cmdName;

    CommandData();

    CommandData(std::string cmdName, std::unordered_map<std::string, std::string> cmdArgs);

    bool exists(std::string key);

    int size();
};
