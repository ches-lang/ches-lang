#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "console.cpp"



class FileManager {

public:

    FileManager() {}

    /*void readBin(std::string path) {
        std::ifstream ifs(path, std::ios::binary);
        if(!ifs.is_open())
            error("specified file '" + path + "' was not found", true);
        if(ifs.fail())
            error("file read error", true);

        char *c;
        while(!ifs.eof()) {
            ifs.read((char*)&c, sizeof(unsigned char));
            std::cout << (long)c << " "; 
        }

        ifs.close();
    }*/

    std::string readText(std::string path) {
        std::ifstream ifs(path);
        if(!ifs.is_open())
            error("specified file '" + path + "' was not found", true);
        if(ifs.fail())
            error("file read error", true);

        std::string res = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

        ifs.close();
        return res;
    }

    void write(std::string path, std::vector<std::vector<std::vector<unsigned char>>> src) {
        std::ofstream ofs;
        ofs.open(path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!ofs) error("file open error", true);

        for(int i = 0; i < src.size(); i++) {
            src[i].push_back({0x00});
            for(int j = 0; j < src[i].size(); j++) {
                if(j < src[i].size() - 1)
                    src[i][j].push_back(0x01);
            }
        }

        for(std::vector<std::vector<unsigned char>> vec1 : src)
            for(std::vector<unsigned char> vec2 : vec1)
                for(unsigned char uc : vec2)
                    ofs.write((char*)&uc, sizeof(char));

        ofs.close();
    }
};



/*
43 4F 4D 50 49 4C 45 44 5F 43 48 45 53 01 00
4E C0 01 00
81 9D 70 72 69 6E 74 6C 6E
*/
