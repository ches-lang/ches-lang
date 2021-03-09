/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
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

        CompilerException() noexcept;

        CompilerException(CompilerExceptionType type) noexcept;

        CompilerException(CompilerExceptionType type, std::unordered_map<std::string, std::string> detailMap) noexcept;
    };


    class Compiler {
    private:
        std::string sourcePath = "";
        std::vector<SourceFile> sourceFiles;

        static std::string cpegRelativePath;

    public:
        /*
         * excep: Compiler::getSourceFiles()
         */
        Compiler(std::string sourcePath);

        /*
         * excep: CompilerException [NoInputFile]
         */
        void compile(std::string outputFilePath);

    private:
        unsigned char* getBytecode();

        /*
         * excep: FileManager::getAllFilePathsInDirectory(std::string) / FileManager::matchExtensionName(std::string, std::string) / SourceFile::loadSourceFile()
         */
        std::vector<SourceFile> getSourceFiles();
    };
}
