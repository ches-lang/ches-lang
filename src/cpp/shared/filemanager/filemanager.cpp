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
        FileManagerException_FileUnopenable,
        FileManagerException_InvalidPath,
        FileManagerException_NotFilePath,
        FileManagerException_PathNotFound
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

        static std::vector<std::string> getAllFilePathsInDirectory(std::string dirPath) {
            std::vector<std::string> pathsInDir = FileManager::getPathsInDirectory(dirPath);
            std::vector<std::string> filePaths;

            for(std::string path : pathsInDir) {
                if(FileManager::isDirectory(path)) {
                    std::vector<std::string> pathsInSubDir = FileManager::getAllFilePathsInDirectory(path);

                    for(std::string subPath : pathsInSubDir)
                        filePaths.push_back(subPath);
                } else {
                    filePaths.push_back(path);
                }
            }

            return filePaths;
        }

        static std::vector<std::string> getFilePathsInDirectory(std::string dirPath) {
            std::vector<std::string> dirPaths = FileManager::getPathsInDirectory(dirPath);
            std::vector<std::string> filePaths;

            for(auto path : dirPaths)
                if(!FileManager::isDirectory(path))
                    filePaths.push_back(path);

            return filePaths;
        }

        static std::vector<std::string> getPathsInDirectory(std::string dirPath) {
            std::vector<std::string> paths;
            auto dirItr = std::filesystem::directory_iterator(dirPath);

            for(auto item : dirItr)
                paths.push_back(item.path());

            return paths;
        }

        inline static bool isDirectory(std::string path) {
            bool isDir;

            try {
                isDir = std::filesystem::is_directory(path);
            } catch(std::filesystem::filesystem_error excep) {
                return false;
            }

            return isDir;
        }

        /*
         * excep: FileManagerException [NotFilePath, PathNotFound, FileUnopenable, Unknown]
         */
        static std::string getText(std::string filePath) {
            std::string text;

            try {
                std::vector<std::string> fileLines = FileManager::getLines(filePath);

                for(const std::string line : fileLines)
                    text += line + "\n";
            } catch(std::exception) {
                throw FileManagerException(FileManagerException_Unknown, filePath);
            }

            return text;
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

        static bool matchExtensionName(std::string filePath, std::string extName) {
            if(FileManager::isDirectory(filePath))
                return false;

            int periodPos = filePath.rfind(".");

            if(periodPos == std::string::npos)
                return false;

            return filePath.substr(periodPos + 1) == extName;
        }

        /*
         * excep: FileManagerException [NotFilePath, PathNotFound, FileUnopenable, Unknown]
         */
        static void writeLines(std::string filePath, std::vector<std::string> lineVec) {
            try {
                if(!FileManager::exists(filePath))
                    throw FileManagerException(FileManagerException_PathNotFound, filePath);

                if(FileManager::isDirectory(filePath))
                    throw FileManagerException(FileManagerException_NotFilePath, filePath);

                std::ofstream ofs(filePath);

                if(!ofs.is_open())
                    throw FileManagerException(FileManagerException_FileUnopenable, filePath);

                for(std::string line : lineVec)
                    ofs << line << std::endl;
            } catch(std::exception) {
                throw FileManagerException(FileManagerException_Unknown, filePath);
            }
        }
    };
}
