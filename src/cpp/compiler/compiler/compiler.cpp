/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
 *
 * Copyright © 2019-2021 Garnet3106 All rights reserved.
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

        CompilerException() noexcept;

        CompilerException(CompilerExceptionType type) noexcept;

        CompilerException(CompilerExceptionType type, std::unordered_map<std::string, std::string> detailMap) noexcept;
    };


    class Compiler {
    private:
        CPEG cpeg;
        std::vector<SourceFile> sourceFiles;

    public:
        /*
         * excep: CPEG::loadCPEGFile(std::string)
         */
        Compiler(std::vector<std::string> srcFilePaths, std::string cpegPath);

        /*
         * excep: CompilerException [NoInputFile]
         */
        unsigned char* compile();

    private:
        unsigned char* getBytecode();
    };
}
