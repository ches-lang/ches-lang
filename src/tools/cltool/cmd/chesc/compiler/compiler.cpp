#pragma once


namespace ches {
    class Compiler {
    public:

        std::vector<std::string> inputFilePaths;
        std::string outputFilePath;

        Compiler();

        void checkOutputFilePath();

        void compile();

        std::vector<std::string> getInputFilePaths();

        ByteSeq toBytecode();
    };
}
