/* 
 * 
 * Chestnut 0.0.0
 * 
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 * 
 * Copyright © 2020 Garnet3106 All rights reserved. 
 * 
 */


#pragma once


namespace ches {
    enum FileErrorType {
        FileError_ReadFailed,
        FileError_FileNotFound,
        FileError_FileOpenFailed
    };


    class FileError : std::exception {
    public:
        FileErrorType type;

        FileError();

        FileError(FileErrorType type);
    };


    class FileManager {
    public:
        inline static bool exists(std::string filePath) {
            return std::filesystem::exists(filePath);
        }

        // spec: パスが見つからない場合はfalseを返す
        inline static bool isDirectory(std::string path) {
            return std::filesystem::is_directory(path);
        }

        // excep: FileError
        static std::string readText(std::string filePath) {
            std::vector<std::string> lines = FileManager::readTextLines(filePath);
            std::string content = "";

            for(int i = 0; i < lines.size(); i++)
                content += lines.at(i) + "\n";

            return content;
        }

        // excep: FileError
        static std::vector<std::string> readTextLines(std::string filePath) {
            if(!FileManager::exists(filePath))
                throw FileError(FileError_FileNotFound);

            if(FileManager::isDirectory(filePath))
                throw FileError(FileError_FileNotFound);

            std::ifstream ifs(filePath);

            if(!ifs.is_open())
                throw FileError(FileError_FileOpenFailed);

            if(ifs.fail())
                throw FileError(FileError_ReadFailed);

            std::vector<std::string> content;
            std::string tmpLine;

            while(std::getline(ifs, tmpLine))
                content.push_back(tmpLine);

            ifs.close();
            return content;
        }
    };
}
