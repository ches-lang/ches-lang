#pragma once


namespace ches {
    class FileManager {
    public:

        static bool exists(std::string filePath) {
            return std::filesystem::exists(filePath);
        }

        static std::string getFileExt(std::string filePath) {
            std::string ext;

            for(int i = filePath.length() - 1; i >= 0; i--) {
                if(filePath[i] == '.') {
                    break;
                } else {
                    ext = filePath[i] + ext;
                }
            }

            return ext;
        }

        static std::string getFileName(std::string filePath) {
            std::filesystem::path pathObj = filePath;
            return pathObj.filename();
        }

        inline static std::vector<std::string> getFilePaths(std::string dirPath) {
            std::vector<std::string> filePaths;

            try {
                std::filesystem::directory_iterator itr(dirPath), end;
                std::error_code err;

                if(!FileManager::exists(dirPath))
                    Console::log(LogType_Error, 327, { { "Path", dirPath } }, true);

                for(; itr != end && !err; itr.increment(err)) {
                    const std::filesystem::directory_entry entry = *itr;
                    filePaths.push_back(entry.path().string());
                }

                if(err)
                    Console::log(LogType_Error, 6845, { { "Directory", dirPath }, { "message", err.message() }}, true);

            } catch(std::filesystem::filesystem_error fse) {
                Console::log(LogType_Error, 6845, { { "Directory", dirPath } }, true);
            }

            return filePaths;
        }

        inline static std::string getFullPath(std::string path) {
            return std::filesystem::absolute(path);
        }

        inline static bool isDirectory(std::string path) {
            return std::filesystem::is_directory(path);
        }

        static ByteSeq readByteSeq(std::string filePath) {
            try {
                ByteSeq fileCont;
                std::ifstream ifs(filePath);

                if(!ifs.is_open())
                    Console::log(LogType_Error, 327, { { "Path", filePath } }, true);

                if(ifs.fail())
                    Console::log(LogType_Error, 6845, { { "Path", filePath } }, true);

                Byte byte;

                do {
                    ifs.read((char*)&byte, sizeof(char));
                    fileCont.push_back(byte);
                } while(!ifs.eof());

                if(fileCont.size() > 0)
                    fileCont.pop_back();

                ifs.close();

                return fileCont;

            } catch(std::exception excep) {
                Console::log(LogType_Error, 6845, { { "Path", filePath } }, true);
            }

            return ByteSeq();
        }

        static void readText(std::string filePath, std::string &fileCont) {
            try {
                std::ifstream ifs(filePath);

                if(!ifs.is_open())
                    Console::log(LogType_Error, 327, { { "Path", filePath } }, true);

                if(ifs.fail())
                    Console::log(LogType_Error, 6845, { { "Path", filePath } }, true);

                fileCont = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
                ifs.close();
            } catch(std::exception excep) {
                Console::log(LogType_Error, 6845, { { "Path", filePath } }, true);
            }
        }

        static std::vector<std::string> readTextLine(std::string filePath) {
            try {
                std::ifstream ifs(filePath);

                if(!ifs.is_open())
                    Console::log(LogType_Error, 327, { { "Path", filePath } }, true);

                if(ifs.fail())
                    Console::log(LogType_Error, 6845, { { "Path", filePath } }, true);

                std::vector<std::string> fileCont;
                std::string line;

                while(std::getline(ifs, line))
                    fileCont.push_back(line);

                ifs.close();
                return fileCont;

            } catch(std::exception excep) {
                Console::log(LogType_Error, 6845, { { "Path", filePath } }, true);
            }

            return {};
        }

        static std::string replacePathExt(std::string filePath, std::string newExt) {
            std::string newFilePath;
            std::string oldExt = FileManager::getFileExt(filePath);

            if(filePath == newExt) {
                newFilePath = filePath + "." + oldExt;
            } else {
                newFilePath = filePath.substr(0, filePath.length() - oldExt.length()) + newExt;
            }

            return newFilePath;
        }

        static void writeBytecode(std::string filePath, ByteSeq source) {
            try {
                std::ofstream ofs;
                ofs.open(filePath, std::ios::out | std::ios::binary | std::ios::trunc);

                if(!ofs)
                    Console::log(LogType_Error, 5352, { { "Path", filePath } }, true);

                for(Byte uc : source)
                    ofs.write((char*)&uc, sizeof(Byte));

                ofs.close();

            } catch(std::exception excep) {
                Console::log(LogType_Error, 5352, { { "Path", filePath } }, true);
            }
        }
    };
}
