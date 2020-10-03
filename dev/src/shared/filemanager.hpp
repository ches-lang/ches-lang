#pragma once


class FileManager {

public:

    static bool exists(std::string path) {
        return std::filesystem::exists(path);
    }

    static std::string getFileExt(std::string path) {
        std::string ext;

        for(int i = path.length() - 1; i >= 0; i--) {
            if(path[i] == '.') break;
            else ext = path[i] + ext;
        }

        return ext;
    }

    static std::string getFileName(std::string path) {
        std::filesystem::path convedPath = path;
        return convedPath.filename();
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

    static ByteSeq readByteSeq(std::string path) {
        try {
            ByteSeq fileCont;

            std::ifstream ifs(path);

            if(!ifs.is_open())
                Console::log(LogType_Error, 327, { { "Path", path } }, true);

            if(ifs.fail())
                Console::log(LogType_Error, 6845, { { "Path", path } }, true);

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
            Console::log(LogType_Error, 6845, { { "Path", path } }, true);
        }

        return ByteSeq();
    }

    static std::string readText(std::string path) {
        try {
            std::ifstream ifs(path);

            if(!ifs.is_open())
                Console::log(LogType_Error, 327, { { "Path", path } }, true);

            if(ifs.fail())
                Console::log(LogType_Error, 6845, { { "Path", path } }, true);

            std::string fileCont = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

            ifs.close();
            return fileCont;
        } catch(std::exception excep) {
            Console::log(LogType_Error, 6845, { { "Path", path } }, true);
        }

        return "";
    }

    static std::vector<std::string> readTextLine(std::string path) {
        try {
            std::ifstream ifs(path);

            if(!ifs.is_open())
                Console::log(LogType_Error, 327, { { "Path", path } }, true);

            if(ifs.fail())
                Console::log(LogType_Error, 6845, { { "Path", path } }, true);

            std::vector<std::string> fileCont;
            std::string line;

            while(std::getline(ifs, line))
                fileCont.push_back(line);

            ifs.close();
            return fileCont;

        } catch(std::exception excep) {
            Console::log(LogType_Error, 6845, { { "Path", path } }, true);
        }

        return {};
    }

    static std::string replacePathExt(std::string path, std::string newExt) {
        std::string newPath;
        std::string oldExt = FileManager::getFileExt(path);

        if(path == newExt) {
            newPath = path + "." + oldExt;
        } else {
            newPath = path.substr(0, path.length() - oldExt.length()) + newExt;
        }

        return newPath;
    }

    static void writeBytecode(std::string path, ByteSeq source) {
        try {
            std::ofstream ofs;
            ofs.open(path, std::ios::out | std::ios::binary | std::ios::trunc);

            if(!ofs)
                Console::log(LogType_Error, 5352, { { "Path", path } }, true);

            for(Byte uc : source)
                ofs.write((char*)&uc, sizeof(Byte));

            ofs.close();

        } catch(std::exception excep) {
            Console::log(LogType_Error, 5352, { { "Path", path } }, true);
        }
    }
};
