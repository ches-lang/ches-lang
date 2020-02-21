#pragma once

#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <string>
#include <uuid/uuid.h>
#include <vector>
#include "parser.cpp"
#include "syntax.cpp"

typedef unsigned char           Byte;
typedef std::vector<Byte>       ByteSeq;
typedef std::vector<ByteSeq>    TokenSeq;
typedef std::vector<TokenSeq>   LineSeq;

#define LN(i)           (bytecode.at(i))
#define TK(i, j)        (bytecode.at(i).at(j))
#define CH(i, j, k)     (bytecode.at(i).at(j).at(k))

#define MAGIC_NUMBER    ByteSeq { 0x63, 0x68, 0x65, 0x73, 0x5F, 0x62, 0x69, 0x6E }



typedef struct FuncData_ {

    std::vector<unsigned char> id;
    std::vector<unsigned char> name;
    std::vector<std::vector<std::vector<unsigned char>>> source;
    int start;
    int end;

    FuncData_();

    FuncData_(std::vector<unsigned char> i, std::vector<unsigned char> nm);

    FuncData_(std::vector<unsigned char> i, std::vector<unsigned char> nm, int st, int ed);

    static FuncData_ findById(std::vector<FuncData_> fd, std::vector<unsigned char> id) {
        for(FuncData_ f : fd)
            if(f.id == id)
                return f;
        return FuncData_();
    }

    static FuncData_ findByName(std::vector<FuncData_> fd, std::vector<unsigned char> nm) {
        for(FuncData_ f : fd)
            if(f.name == nm)
                return f;
        return FuncData_();
    }

} FuncData;



//relpath
class Bytecode {

public:

    std::vector<unsigned char> source;

    Bytecode();

    Bytecode(std::vector<unsigned char> src);

    Bytecode(int src);

    Bytecode(std::string src);

    Bytecode(Node tree);

    Bytecode(std::vector<Bytecode> src);

    Bytecode(std::vector<std::vector<std::vector<unsigned char>>> src);

    Bytecode append(unsigned char src);

    Bytecode append(Bytecode src);

    Bytecode pop();

    int toInt();

    std::string toString();

    // vector型の命令を取得
    std::vector<std::vector<std::vector<unsigned char>>> divide();

private:

    int index = 0;
    std::vector<std::vector<std::vector<unsigned char>>> lines;
    std::vector<FuncData> funcdata;
    std::string spaceName;
    std::string className;
    int lslen = 0;
    int lllen = 0;

    Bytecode toBytecode(Node tree);

    void addBytecodeToNode(Node node);

    unsigned char generateUUID();
};
