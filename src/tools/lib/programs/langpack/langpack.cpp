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
    typedef std::unordered_map<int, std::string>  log_prop_map;


    class LangPack {
    public:
        LangPack();

    private:
        void load() {
            FileManager::read();
        }
    };
}
