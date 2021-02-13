/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


namespace ches::shared {
    enum FileManagerExceptionType {
        FileManagerException_Unknown,
        FileManagerException_NotFilePath,
        FileManagerException_PathNotFound,
        FileManagerException_FileUnopenable
    };


    class FileManagerException : std::exception {
    public:
        FileManagerExceptionType type = FileManagerException_Unknown;
        std::string target = "";

        FileManagerException();

        FileManagerException(FileManagerExceptionType type);

        FileManagerException(FileManagerExceptionType type, std::string target);
    };


    class FileManager {
    public:
        inline static bool exists(std::string path) {
            return std::filesystem::exists(path);
        }

        static std::vector<std::string> getFilePathsInDirectory(std::string dirPath) {
            std::vector<std::string> filePaths;
            auto dirItr = std::filesystem::directory_iterator(dirPath);

            for(auto file : dirItr)
                filePaths.push_back(file.path());

            return filePaths;
        }

        inline static bool isDirectory(std::string path) {
            return std::filesystem::is_directory(path);
        }

        /*
         * excep: FileManagerException [NotFilePath, PathNotFound, FileUnopenable, Unknown]
         */
        static std::vector<std::string> getLines(std::string filePath) {
            std::vector<std::string> lineVec;

            try {
                if(!FileManager::exists(filePath))
                    throw FileManagerException(FileManagerException_PathNotFound, filePath);

                if(FileManager::isDirectory(filePath))
                    throw FileManagerException(FileManagerException_NotFilePath, filePath);

                std::ifstream ifs(filePath);
                std::string line_tmp;

                if(!ifs.is_open())
                    throw FileManagerException(FileManagerException_FileUnopenable, filePath);

                while(getline(ifs, line_tmp))
                    lineVec.push_back(line_tmp);
            } catch(std::exception) {
                throw FileManagerException(FileManagerException_Unknown, filePath);
            }

            return lineVec;
        }
    };
}
