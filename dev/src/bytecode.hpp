#pragma once

#define LN(i)           (bytecode.at(i))
#define TK(i, j)        (bytecode.at(i).at(j))
#define CH(i, j, k)     (bytecode.at(i).at(j).at(k))

#define MAGIC_NUMBER    ByteSeq { 0x4F, 0x52, 0x49, 0x4E, 0x43, 0x48, 0x41, 0x4E }



struct FuncData {

    ByteSeq id;
    ByteSeq name;
    LineSeq source;
    int start;
    int end;

    FuncData();

    FuncData(ByteSeq i, ByteSeq nm);

    FuncData(ByteSeq i, ByteSeq nm, int st, int ed);

    static FuncData findById(std::vector<FuncData> fd, ByteSeq id) {
        for(FuncData f : fd)
            if(f.id == id)
                return f;
        return FuncData();
    }

    static FuncData findByName(std::vector<FuncData> fd, ByteSeq nm) {
        for(FuncData f : fd)
            if(f.name == nm)
                return f;
        return FuncData();
    }

};



struct Instruction {

    Byte opcode = IT_Unknown;
    std::unordered_map<std::string, ByteSeq> operand;
    ByteSeq bytecode;

    Instruction(int opcode);

    Instruction(int opcode, std::unordered_map<std::string, ByteSeq> operand);

    void setBytecode();

    void append(Byte byte);

    void append(ByteSeq bytes);

};



typedef std::vector<Instruction>    InstList;



//relpath
struct Bytecode {

    ByteSeq source;

    Bytecode();

    Bytecode(ByteSeq src);

    Bytecode(int src);

    Bytecode(std::string src);

    Bytecode(Node tree);

    Bytecode(std::vector<Bytecode> src);

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

    InstList toInstList(Node parentNode, int &index);

    Byte generateUUID();
};
