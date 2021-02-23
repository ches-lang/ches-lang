/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


namespace ches::compiler {
    enum CompilerExceptionType {
        CompilerException_Unknown,
        CompilerException_NoInputFile
    };


    class CompilerException {
    public:
        CompilerExceptionType type;
        std::unordered_map<std::string, std::string> detailMap;

        CompilerException();

        CompilerException(CompilerExceptionType type);

        CompilerException(CompilerExceptionType type, std::unordered_map<std::string, std::string> detailMap);
    };


    class Compiler {
    public:
        std::string sourcePath = "";
        std::vector<SourceFile> sourceFiles;

        Compiler(std::string sourcePath);

        void compile(std::string outputFilePath);

    private:
        unsigned char* getBytecode();

        std::vector<SourceFile> getSourceFiles();
    };
}
