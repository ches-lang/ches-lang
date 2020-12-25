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
    class Compiler {
    public:
        std::string sourceCode;

        Compiler();

        Compiler(std::string sourceCode);

        byte* compile();
    };
}
