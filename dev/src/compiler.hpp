#pragma once



class Compiler {

public:

    std::string path;

    Compiler();

    void compile(std::string path);

    ByteSeq getByteSeq();
};
