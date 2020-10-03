#pragma once


struct Command : std::unordered_map<std::string, std::string> {
    std::string cmdName;

    Command();

    Command(int argc, char *argv[]);

    void endCount(timepoint start);

    bool existsKey(std::string key);

    void run();

    void setArgs(int argc, char *argv[]);

    int size();

    timepoint startCount();
};
