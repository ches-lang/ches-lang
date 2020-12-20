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
        static void readText(std::string filePath, std::string &content) {
            if(!FileManager::exists(filePath))
                throw FileError(FileError_FileNotFound);

            if(FileManager::isDirectory(filePath))
                throw FileError(FileError_FileNotFound);

            std::ifstream ifs(filePath);

            if(!ifs.is_open())
                throw FileError(FileError_FileOpenFailed);

            if(ifs.fail())
                throw FileError(FileError_ReadFailed);

            std::string tmpLine;

            while(std::getline(ifs, tmpLine))
                content += tmpLine + "\n";

            ifs.close();
        }
    };
}
