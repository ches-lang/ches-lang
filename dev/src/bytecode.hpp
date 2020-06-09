#pragma once

#define LN(i)           (bytecode.at(i))
#define TK(i, j)        (bytecode.at(i).at(j))
#define CH(i, j, k)     (bytecode.at(i).at(j).at(k))

#define HEADER_LEN      128
#define MAGIC_NUMBER    ByteSeq { 0x4F, 0x52, 0x49, 0x4E, 0x43, 0x48, 0x41, 0x4E }



struct Instruction {

    Byte opcode = IT_Unknown;
    std::unordered_map<std::string, ByteSeq> operand;
    ByteSeq bytecode;

    Instruction(ByteSeq bytes);

    Instruction(int opcode);

    Instruction(int opcode, std::unordered_map<std::string, ByteSeq> operand);

    void setBytecode();

    ByteSeq copyBytecode(int begin, int end);

    void append(Byte byte);

    void append(ByteSeq bytes);

};

typedef std::vector<Instruction>    InstList;



struct HeaderInfo {

    ByteSeq magicNum;

    HeaderInfo();

    HeaderInfo(ByteSeq bytes);

    ByteSeq toByteSeq();

private:

    void append(ByteSeq &src, ByteSeq bytes);

};



struct FuncData {

    ByteSeq id;
    ByteSeq name;
    InstList instList;
    int begin;
    int end;

    FuncData();

    FuncData(ByteSeq id, ByteSeq name);

    FuncData(ByteSeq id, ByteSeq name, int begin, int end);

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



//relpath
struct Bytecode {

    ByteSeq source;

    Bytecode();

    Bytecode(ByteSeq src);

    Bytecode(int src);

    Bytecode(std::string src);

    Bytecode(Node tree, std::string filePath, std::string sourceCode);

    Bytecode(std::vector<Bytecode> src);

    Bytecode append(Byte src);

    Bytecode append(Bytecode src);

    Bytecode pop();

    std::string toHexString();

    int toInt();

    std::string toString();

    TokenSeq divide();

private:

    LineSeq lines;
    std::string filePath;
    std::string sourceCode;
    std::vector<FuncData> funcData;
    std::string spaceName;
    std::string className;
    int lslen = 0;
    int lllen = 0;

    InstList toInstList(Node node);

    InstList toInstList(Node parentNode, int &index);

    ByteSeq generateUUID();
};
