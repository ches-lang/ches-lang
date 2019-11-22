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

    void write(std::string path, bytecode src) {
        std::ofstream ofs;
        ofs.open(path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!ofs) error("file open error", true);

        for(unsigned char uc : src)
            ofs.write((char*)&uc, sizeof(char));

        ofs.close();
    }
};



/*
43 4F 4D 50 49 4C 45 44 5F 43 48 45 53 01 00
4E C0 01 00
81 9D 70 72 69 6E 74 6C 6E
*/
