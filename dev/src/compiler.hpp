#pragma once



class Compiler {

public:

    Options options;

    Compiler();

    Compiler(Options opt);

    void compile();

    ByteSeq getByteSeq();
};
