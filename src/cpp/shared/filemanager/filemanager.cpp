/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
 *
 * Copyright © 2019-2021 Garnet3106 All rights reserved.
 */


#pragma once


namespace ches::shared {
    enum FileManagerExceptionType {
        FileManagerException_FileUnopenable,
        FileManagerException_InvalidPath,
        FileManagerException_NotFilePath,
        FileManagerException_PathNotFound,
        FileManagerException_Unknown,
    };


    class FileManagerException : std::exception {
    public:
        FileManagerExceptionType type = FileManagerException_Unknown;
        std::string target = "";

        FileManagerException() noexcept;

        FileManagerException(FileManagerExceptionType type) noexcept;

        FileManagerException(FileManagerExceptionType type, std::string target) noexcept;
    };


    class FileManager {
    public:
        /*
         * excep: FileManagerException [Unknown]
         */
        inline static bool exists(std::string path) {
            bool exists;

            try {
                exists = std::filesystem::exists(path);
            } catch(std::filesystem::filesystem_error excep) {
                throw FileManagerException(FileManagerException_Unknown, path);
            }

            return exists;
        }

        /*
         * excep: FileManagerException [Unknown]
         */
        static std::vector<std::string> getAllFilePathsInDirectory(std::string dirPath) {
            std::vector<std::string> pathsInDir;
            std::vector<std::string> filePaths;

            try {
                pathsInDir = FileManager::getPathsInDirectory(dirPath);

                for(std::string path : pathsInDir) {
                    if(FileManager::isDirectory(path)) {
                        std::vector<std::string> pathsInSubDir = FileManager::getAllFilePathsInDirectory(path);

                        for(std::string subPath : pathsInSubDir)
                            filePaths.push_back(subPath);
                    } else {
                        filePaths.push_back(path);
                    }
                }
            } catch(std::filesystem::filesystem_error excep) {
                throw FileManagerException(FileManagerException_Unknown, excep.path1().string());
            }

            return filePaths;
        }

        /*
         * excep: FileManagerException [Unknown]
         */
        static std::vector<std::string> getFilePathsInDirectory(std::string dirPath) {
            std::vector<std::string> dirPaths;
            std::vector<std::string> filePaths;

            try {
                dirPaths = FileManager::getPathsInDirectory(dirPath);

                for(auto path : dirPaths)
                    if(!FileManager::isDirectory(path))
                        filePaths.push_back(path);
            } catch(std::filesystem::filesystem_error excep) {
                throw FileManagerException(FileManagerException_Unknown, excep.path1().string());
            }

            return filePaths;
        }

        /*
         * excep: FileManagerException [Unknown]
         */
        static std::vector<std::string> getPathsInDirectory(std::string dirPath) {
            std::vector<std::string> paths;
            std::filesystem::directory_iterator dirItr;

            try {
                dirItr = std::filesystem::directory_iterator(dirPath);
            } catch(std::filesystem::filesystem_error excep) {
                throw FileManagerException(FileManagerException_Unknown, dirPath);
            }

            for(auto item : dirItr)
                paths.push_back(item.path().string());

            return paths;
        }

        /*
         * excep: FileManagerException [Unknown]
         */
        inline static bool isDirectory(std::string path) {
            bool isDir;

            try {
                isDir = std::filesystem::is_directory(path);
            } catch(std::filesystem::filesystem_error excep) {
                throw FileManagerException(FileManagerException_Unknown, path);
            }

            return isDir;
        }

        /*
         * excep: FileManager::getLines(std::string)
         */
        static std::string getText(std::string filePath) {
            std::string text = "";

            try {
                std::vector<std::string> fileLines = FileManager::getLines(filePath);

                for(const std::string line : fileLines)
                    text += line + "\n";
            } catch(FileManagerException excep) {
                throw excep;
            }

            return text;
        }

        /*
         * excep: FileManagerException [NotFilePath, PathNotFound, FileUnopenable] / FileManager::exists(std::string) / FileManager::isDirectory(std::string)
         */
        static std::vector<std::string> getLines(std::string filePath) {
            std::vector<std::string> fileLines;

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
                    fileLines.push_back(line_tmp);
            } catch(FileManagerException excep) {
                throw excep;
            }

            return fileLines;
        }

        /*
         * excep: FileManager::isDirectory(std::string)
         */
        static bool matchExtensionName(std::string filePath, std::string extName) {
            try {
                if(FileManager::isDirectory(filePath))
                    return false;
            } catch(FileManagerException excep) {
                throw excep;
            }

            int periodPos = filePath.rfind(".");

            if(periodPos == std::string::npos)
                return false;

            return filePath.substr(periodPos + 1) == extName;
        }

        /*
         * excep: FileManagerException [NotFilePath, PathNotFound, FileUnopenable] / FileManager::exists(std::string) / FileManager::isDirectory(std::string)
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
            } catch(FileManagerException excep) {
                throw excep;
            }
        }
    };
}
