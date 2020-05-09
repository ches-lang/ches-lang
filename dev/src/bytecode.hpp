#pragma once

#define LN(i)           (bytecode.at(i))
#define TK(i, j)        (bytecode.at(i).at(j))
#define CH(i, j, k)     (bytecode.at(i).at(j).at(k))

#define MAGIC_NUMBER    ByteSeq { 0x63, 0x68, 0x65, 0x73, 0x5F, 0x62, 0x69, 0x6E }



typedef struct FuncData_ {

    ByteSeq id;
    ByteSeq name;
    LineSeq source;
    int start;
    int end;

    FuncData_();

    FuncData_(ByteSeq i, ByteSeq nm);

    FuncData_(ByteSeq i, ByteSeq nm, int st, int ed);

    static FuncData_ findById(std::vector<FuncData_> fd, ByteSeq id) {
        for(FuncData_ f : fd)
            if(f.id == id)
                return f;
        return FuncData_();
    }

    static FuncData_ findByName(std::vector<FuncData_> fd, ByteSeq nm) {
        for(FuncData_ f : fd)
            if(f.name == nm)
                return f;
        return FuncData_();
    }

} FuncData;



//relpath
class Bytecode {

public:

    ByteSeq source;

    Bytecode();

    Bytecode(ByteSeq src);

    Bytecode(int src);

    Bytecode(std::string src);

    Bytecode(Node tree);

    Bytecode(std::vector<Bytecode> src);

    Bytecode(LineSeq src);

    Bytecode append(Byte src);

    Bytecode append(Bytecode src);

    Bytecode pop();

    int toInt();

    std::string toString();

    LineSeq divide();

private:

    LineSeq lines;
    std::vector<FuncData> funcdata;
    std::string spaceName;
    std::string className;
    int lslen = 0;
    int lllen = 0;

    Bytecode toBytecode(Node tree);

    void scanNode(Node node);

    LineSeq nodeToBytecode(Node node, int &index);

    Byte generateUUID();
};
