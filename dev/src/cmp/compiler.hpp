#pragma once

class Compiler {

public:

    Command cmd;
    std::string path;

    Compiler();

    Compiler(Command cmd);

    void compile(std::string path);

    ByteSeq getByteSeq();
};
