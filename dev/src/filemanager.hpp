#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>



class FileManager {

public:

    static Bytecode readBytecode(std::string path) {
        try {
            Bytecode res;

            std::ifstream ifs(path);
            if(!ifs.is_open())
                Console::log(LogType_Error, "0327", { { "Path", path } }, true);
            if(ifs.fail())
                Console::log(LogType_Error, "6845", { { "Path", path } }, true);

            unsigned char uc;
            do {
                ifs.read((char*)&uc, sizeof(unsigned char));
                res.source.push_back(uc);
            } while(!ifs.eof());
            res.source.pop_back();

            ifs.close();
            return res;
        } catch(std::exception excep) {
            Console::log(LogType_Error, "6845", { { "Path", path } }, true);
        }
        return Bytecode();
    }

    static std::string readText(std::string path) {
        try {
            std::ifstream ifs(path);

            if(!ifs.is_open())
                Console::log(LogType_Error, "0327", { { "Path", path } }, true);

            if(ifs.fail())
                Console::log(LogType_Error, "6845", { { "Path", path } }, true);

            std::string res = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

            ifs.close();
            return res;
        } catch(std::exception excep) {
            Console::log(LogType_Error, "6845", { { "Path", path } }, true);
        }

        return "";
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

    static void writeBytecode(std::string path, Bytecode src) {
        try {
            std::ofstream ofs;
            ofs.open(path, std::ios::out | std::ios::binary | std::ios::trunc);

            if (!ofs)
                Console::log(LogType_Error, "5352", { { "Path", path } }, true);

            for(unsigned char uc : src.source)
                ofs.write((char*)&uc, sizeof(char));

            ofs.close();
        } catch(std::exception excep) {
            Console::log(LogType_Error, "5352", { { "Path", path } }, true);
        }
    }

    static bool exists(std::string path) {
        return std::filesystem::exists(path);
    }

    static bool isDirectory(std::string path) {
        return std::filesystem::is_directory(path);
    }

    static std::string getName(std::string path) {
        std::vector<std::string> divided = { "" };

        for(char ch : path) {
            if(ch == '/') {
                divided.push_back("");
            } else {
                divided[divided.size() - 1] += ch;
            }
        }

        for(int i = divided.size() - 1; i > 0; i--) {
            if(divided[i] != "") {
                return divided[i];
            }
        }

        return "";
    }

    static std::vector<std::string> getFilePaths(std::string dirpath) {
        std::vector<std::string> filepaths;
        try {
            std::filesystem::directory_iterator iter(dirpath), end;
            std::error_code err;

            if(!FileManager::exists(dirpath))
                Console::log(LogType_Error, "0327", { { "Path", dirpath } }, true);

            for(; iter != end && !err; iter.increment(err)) {
                const std::filesystem::directory_entry entry = *iter;
                filepaths.push_back(entry.path().string());
            }

            if(err)
                Console::log(LogType_Error, "6845", { { "Directory", dirpath }, { "message", err.message() }}, true);

        } catch(std::filesystem::filesystem_error fse) {
            Console::log(LogType_Error, "6845", { { "Directory", dirpath } }, true);
        }

        return filepaths;
    }

    static std::string getFileName(std::string path) {
        std::filesystem::path convedPath = path;
        return convedPath.filename();
    }

    static std::string getFileExt(std::string path) {
        std::string ext;

        for(int i = path.length() - 1; i >= 0; i--)
            if(path[i] == '.') break;
            else ext = path[i] + ext;

        return ext;
    }

    static std::string getFullPath(std::string path) {
        return std::filesystem::absolute(path);
    }

    static std::string renamePathExt(std::string path, std::string newext) {
        std::string newpath;
        std::string oldext = FileManager::getFileExt(path);

        if(path == newext) newpath = path + "." + oldext;
        else newpath = path.substr(0, path.length() - oldext.length()) + newext;
        return newpath;
    }
};
