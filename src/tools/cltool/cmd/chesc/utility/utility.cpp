#pragma once


namespace ches {
    #define HEADER_LEN      128
    #define MAGIC_NUMBER    ByteSeq { 0x4F, 0x52, 0x49, 0x4E, 0x43, 0x48, 0x41, 0x4E }

    struct ByteSeq;
    struct LineSeq;
    struct FuncList;
    struct HeaderInfo;


    /* ! ? ~ + - * / % ^ = | & . , : ; ( ) [ ] < > { } */
    enum TokenType : Byte {
        TK_Unknown,
        TK_EndOfFile,
        TK_NewLine,
        TK_Comment,
        TK_Keyword,
        TK_Identifier,
        TK_Number,
        TK_Char,
        TK_String,
        TK_Exclamation,
        TK_Question,
        TK_Tilde,
        TK_Plus,
        TK_Hyphen,
        TK_Asterisk,
        TK_Slash,
        TK_Percent,
        TK_Caret,
        TK_Equal,
        TK_Pipe,
        TK_Ampersand,
        TK_Period,
        TK_Comma,
        TK_Colon,
        TK_Semicolon,
        TK_LeftParen,
        TK_RightParen,
        TK_LeftBracket,
        TK_RightBracket,
        TK_LeftAngleBracket,
        TK_RightAngleBracket,
        TK_LeftBrace,
        TK_RightBrace
    };


    enum NodeType : Byte {
        ND_Unknown,
        ND_Root,
        ND_Value,
        ND_Object,
        ND_DefVar,
        ND_InitVar,
        ND_DefFunc,
        ND_CallFunc,
        ND_Args,
        ND_If,
        ND_Else,
        ND_ElseIf,
        ND_Loop,
        ND_Count,
        ND_Logic,
        ND_Compare,
        ND_Expression,
        ND_Operator,
        ND_Equal,
        ND_Less,
        ND_Greater,
        ND_LessEqual,
        ND_GreaterEqual
    };


    enum InstType : Byte {
        IT_Unknown,
        IT_InstDiv,
        IT_OpeDiv,
        IT_Add,
        IT_And,
        IT_ArgPref,
        IT_Block,
        IT_Div,
        IT_Equal,
        IT_Greater,
        IT_Join,
        IT_Jump,
        IT_JumpIf,
        IT_JumpIfNot,
        IT_Load,
        IT_Mul,
        IT_Or,
        IT_Pop,
        IT_Push,
        IT_Return,
        IT_Sub,
        IT_SysCall
    };


    enum ValueType : Byte {
        VT_Unknown,
        VT_Boolean,
        VT_Byte,
        VT_UByte,
        VT_Short,
        VT_UShort,
        VT_Int,
        VT_UInt,
        VT_Long,
        VT_ULong,
        VT_Char,
        VT_String,
        VT_Decimal
    };


    enum DataSizeType : Byte {
        DST_2,
        DST_8,
        DST_16,
        DST_32,
        DST_64
    };


    // tokenTypeMapを追加する


    std::unordered_map<Byte, std::string> nodeTypeMap {
        { ND_Unknown, "Unknown" },
        { ND_Root, "Root" },
        { ND_Value, "Value" },
        { ND_DefVar, "DefVar" },
        { ND_InitVar, "InitVar" },
        { ND_DefFunc, "DefFunc" },
        { ND_CallFunc, "CallFunc" },
        { ND_Args, "Args" },
        { ND_If, "If" },
        { ND_Else, "Else" },
        { ND_ElseIf, "ElseIf" },
        { ND_Loop, "Loop" },
        { ND_Count, "Count" },
        { ND_Logic, "Logic" },
        { ND_Compare, "Comp" },
        { ND_Expression, "Exp" },
        { ND_Operator, "Ope" },
        { ND_Equal, "Equal" },
        { ND_Less, "Less" },
        { ND_Greater, "Greater" },
        { ND_LessEqual, "LessEqual" },
        { ND_GreaterEqual, "GreaterEqual" },
    };


    std::unordered_map<Byte, std::string> instTypeMap {
        { IT_Unknown, "Unknown" },
        { IT_InstDiv, "InstDiv" },
        { IT_OpeDiv, "OpeDiv" },
        { IT_Add, "Add" },
        { IT_And, "And" },
        { IT_ArgPref, "ArgPref" },
        { IT_Block, "Block" },
        { IT_Div, "Div" },
        { IT_Equal, "Equal" },
        { IT_Greater, "Greater" },
        { IT_Join, "Join" },
        { IT_Jump, "Jump" },
        { IT_JumpIf, "JumpIf" },
        { IT_JumpIfNot, "JumpIfNot" },
        { IT_Load, "Load" },
        { IT_Mul, "Mul" },
        { IT_Or, "Or" },
        { IT_Pop, "Pop" },
        { IT_Push, "Push" },
        { IT_Return, "Return" },
        { IT_Sub, "Sub" },
        { IT_SysCall, "SysCall" }
    };


    std::unordered_map<Byte, std::string> valueTypeMap {
        { VT_Unknown, "" },
        { VT_Boolean, "bol" },
        { VT_Byte, "byt" },
        { VT_UByte, "ubyt" },
        { VT_Short, "sht" },
        { VT_UShort, "usht" },
        { VT_Int, "int" },
        { VT_UInt, "uint" },
        { VT_Long, "lng" },
        { VT_ULong, "ulng" },
        { VT_Decimal, "dec" },
        { VT_Char, "chr" },
        { VT_String, "str" }
    };


    std::unordered_map<std::string, DataSizeType> dataSizeTypeMap {
        { "bol", DST_2 },
        { "byt", DST_8 },
        { "ubyt", DST_8 },
        { "sht", DST_16 },
        { "usht", DST_16 },
        { "int", DST_32 },
        { "uint", DST_32 },
        { "lng", DST_64 },
        { "ulng", DST_64 },
        { "dec", DST_32 },
        { "chr", DST_8 },
        { "str", DST_32 }
    };


    template<class T>

    struct vector_ext : public std::vector<T> {
        vector_ext() {}

        vector_ext(T value) {
            this->push_back(value);
        }

        vector_ext(std::initializer_list<T> value) {
            for(T val : value)
                this->push_back(val);
        }

        vector_ext(std::vector<T> value) {
            this->push_back(value);
        }

        vector_ext<T> copy(int begin) {
            return this->copy(begin, this->size() - 1);
        }

        vector_ext<T> copy(int begin, int end) {
            vector_ext<T> result;

            auto beginItr = begin >= 0 ? this->begin() + begin : this->begin() + this->size() + begin + 1;
            auto endItr = end >= 0 ? this->end() - this->size() + end + 1 : this->end() + end + 1;

            std::copy(beginItr, endItr, std::back_inserter(result));

            return result;
        }

        vector_ext<vector_ext<T>> divide(T separator) {
            vector_ext<vector_ext<T>> divided = {};

            for(T value : *this) {
                if(value.type == separator) {
                    divided.push_back({});
                } else {
                    divided[divided.size() - 1].push_back(value);
                }
            }

            return divided;
        }

        inline void pop_back() {
            std::vector<T>::pop_back();
        }

        inline void pop_back(int len) {
            for(int i = 0; i < len; i++)
                std::vector<T>::pop_back();
        }

        inline void push_back() {
            std::vector<T>::push_back(T());
        }

        inline void push_back(T value) {
            std::vector<T>::push_back(value);
        }

        inline void push_back(std::vector<T> value) {
            for(T val : value)
                std::vector<T>::push_back(val);
        }

        inline void push_back(vector_ext<T> value) {
            for(T val : value)
                std::vector<T>::push_back(val);
        }

        inline void push_front(T value) {
            std::vector<T>::insert(this->begin(), value);
        }
    };


    struct Token {
        int index = 0;
        Byte type = TK_Unknown;
        std::string string = "";

        Token();

        Token(Byte type);

        Token(std::string string);

        Token(Byte type, std::string string);

        Token(Byte type, std::string string, int index);

        bool compare(Token token);

        Token getCloseParen();

        Token getOpenParen();

        std::pair<int, int> getPosition(std::string &source);

        static std::pair<int, int> getPosition(std::string &source, int index) {
            int line = 0;
            int pos = 0;

            for(int i = 0; i < index; i++) {
                if(source[i] == '\n') {
                    pos = 0;
                    line++;
                } else {
                    pos++;
                }
            }

            return { line, pos };
        }

        static std::string getPositionText(std::string filePath, std::string &source, int index) {
            std::pair pos = Token::getPosition(source, index);
            return filePath + ":" + std::to_string(pos.first + 1) + ":" + std::to_string(pos.second + 1);
        }

        std::string getPositionText(std::string filePath, std::string &source);

        bool isParen();

        bool isCloseParen();

        bool isOpenParen();

        bool isValueType();

        bool match(ByteSeq matches);

        bool match(std::string regexp);
    };


    struct TokenSeq : vector_ext<Token> {
        TokenSeq copy(int begin);

        TokenSeq copy(int begin, int end);

        vector_ext<TokenSeq> divide(TokenType separator);

        inline void pop_back() {
            vector_ext<Token>::pop_back();
        }

        inline void pop_back(int len) {
            for(int i = 0; i < len; i++)
                vector_ext<Token>::pop_back();
        }

        inline void push_back() {
            vector_ext<Token>::push_back(Token());
        }

        inline void push_back(Token value) {
            vector_ext<Token>::push_back(value);
        }

        inline void push_back(TokenSeq value) {
            for(Token val : value)
                vector_ext<Token>::push_back(val);
        }

        inline void push_back(vector_ext<Token> value) {
            for(Token val : value)
                vector_ext<Token>::push_back(val);
        }

        inline void push_front(Token value) {
            vector_ext<Token>::insert(this->begin(), value);
        }
    };


    struct Node {
        Byte type = ND_Unknown;
        vector_ext<Node> children;
        vector_ext<Token> tokens;
        std::string prefix = "||";

        Node();

        Node(Byte type);

        Node(Byte type, std::vector<Node> children);

        Node(Byte type, std::vector<Token> tokens);

        Node(Byte type, std::vector<Node> children, std::vector<Token> tokens);

        void addChild(Node node);

        void addToken(Token token);

        void addToken(TokenSeq tokenSeq);

        Node childAt(int index);

        Token tokenAt(int index);

        void print();

        void print(std::string level);

        std::string typeToString();
    };


    struct ByteSeq : public vector_ext<Byte> {
        ByteSeq();

        ByteSeq(Byte value);

        ByteSeq(Byte *value, int length);

        ByteSeq(Byte value, int len);

        ByteSeq(std::initializer_list<Byte> value);

        ByteSeq(int source);

        ByteSeq(std::string source);

        ByteSeq(Token token);

        ByteSeq(Node node);

        ByteSeq(Node tree, std::string filePath, std::string sourceCode);

        // vector_ext<T>::copy() のカプセル化 (セグフォ防止のため)

        ByteSeq copy(int begin);

        ByteSeq copy(int begin, int end);

        ByteSeq escape();

        static ByteSeq generateUUID() {
            ByteSeq result;

            std::random_device rand;
            std::mt19937 mt(rand());
            std::uniform_int_distribution<> rand256(0, 255);

            for(int i = 0; i < 16; i++)
                result.push_back((Byte)rand256(mt));

            return result;
        }

        static ByteSeq join(vector_ext<ByteSeq> byteSeqs, ByteSeq separator = {}) {
            ByteSeq result;

            for(ByteSeq seq : byteSeqs) {
                result.push_back(seq);
                result.push_back(separator);
            }

            if(result.size() > 0)
                result.pop_back(separator.size());

            return result;
        }

        inline void pop_back() {
            vector_ext<Byte>::pop_back();
        }

        inline void pop_back(int len) {
            for(int i = 0; i < len; i++)
                vector_ext<Byte>::pop_back();
        }

        inline void push_back() {
            vector_ext<Byte>::push_back((Byte)0);
        }

        inline void push_back(Byte *value, int length) {
            for(int i = 0; i < length; i++)
                vector_ext<Byte>::push_back(value[i]);
        }

        inline void push_back(Byte value) {
            vector_ext<Byte>::push_back(value);
        }

        inline void push_back(std::string value) {
            for(char val : value)
                vector_ext<Byte>::push_back(val);
        }

        inline void push_back(ByteSeq value) {
            for(char val : value)
                vector_ext<Byte>::push_back(val);
        }

        inline void push_front(Byte value) {
            vector_ext<Byte>::insert(this->begin(), value);
        }

        int toDataSize();

        std::string toHexString(std::string sep = "");

        int toInt();

        LineSeq toLineSeq();

        void* toNumber(ValueType type);

        Byte* toPointerArray();

        std::string toString();
    };


    struct LineSeq : public vector_ext<ByteSeq> {
        LineSeq();

        LineSeq(ByteSeq value);

        LineSeq(std::initializer_list<ByteSeq> value);

        LineSeq(std::vector<ByteSeq> value);
    };


    struct Instruction {
        Byte opcode = IT_Unknown;
        std::vector<ByteSeq> operand;

        Instruction();

        Instruction(ByteSeq bytes);

        Instruction(int opcode);

        Instruction(int opcode, std::vector<ByteSeq> operand);
    };


    struct InstList : public vector_ext<Instruction> {
        InstList();

        InstList(Instruction value);

        InstList(std::initializer_list<Instruction> value);

        InstList(std::vector<Instruction> value);

        inline void push_back(Instruction value) {
            vector_ext<Instruction>::push_back(value);
        }

        inline void push_back(InstList value) {
            for(Instruction val : value)
                vector_ext<Instruction>::push_back(val);
        }

        ByteSeq toByteSeq();
    };


    struct Function {
        ByteSeq id;
        ByteSeq name;
        InstList instList;
        int begin;
        int end;

        Function();

        Function(ByteSeq id, ByteSeq name);

        Function(ByteSeq id, ByteSeq name, int begin, int end);
    };


    struct FuncList : public vector_ext<Function> {
        FuncList();

        FuncList(Function value);

        FuncList(std::initializer_list<Function> value);

        FuncList(std::vector<Function> value);

        bool existsId(ByteSeq id);

        bool existsName(ByteSeq name);

        Function findById(ByteSeq id);

        Function findByName(ByteSeq name);

        inline void pop_back() {
            vector_ext<Function>::pop_back();
        }

        inline void pop_back(int len) {
            for(int i = 0; i < len; i++)
                vector_ext<Function>::pop_back();
        }

        inline void push_back() {
            vector_ext<Function>::push_back(Function());
        }

        inline void push_back(Function value) {
            vector_ext<Function>::push_back(value);
        }

        inline void push_back(std::vector<Function> value) {
            for(Function val : value)
                vector_ext<Function>::push_back(val);
        }

        inline void push_back(vector_ext<Function> value) {
            for(Function val : value)
                vector_ext<Function>::push_back(val);
        }

        inline void push_front(Function value) {
            vector_ext<Function>::insert(this->begin(), value);
        }

        inline void push_back(FuncList value) {
            for(Function val : value)
                vector_ext<Function>::push_back(val);
        }
    };


    struct HeaderInfo {
        ByteSeq magicNum;

        HeaderInfo();

        HeaderInfo(ByteSeq bytes);

        ByteSeq toByteSeq();
    };


    class InstConv {
    public:
        //InstList instList;
        int index;
        Node tree;

        std::string filePath;
        std::string source;

        FuncList labelList;
        std::string nameSpaceName;
        std::string className;

        int localStackLen = 0;
        int localListLen = 0;

        InstConv();

        DataSizeType toDataSizeType(std::string type) {
            return dataSizeTypeMap.at(type);
        }

        InstList toInstList(Node tree);

        InstList toInstList(Node parent, int &index);

        InstList toInstList(Node tree, std::string filePath, std::string source);

        InstList toInstList_callFunc(Node &node) {
            InstList result;

            Token nameToken = node.tokenAt(0);
            std::string name = nameToken.string;

            ByteSeq id = this->labelList.findByName(ByteSeq(name)).id;

            if(id.size() != 16)
                Console::log(LogType_Error, 1822, { { "At", nameToken.getPositionText(this->filePath, this->source ) }, { "Id", name } }, false);

            Node args = node.childAt(0);

            for(Node node : args.children) {
                if(node.type == ND_Value) {
                    DataSizeType size = InstConv::toDataSizeType(node.tokenAt(0).string);
                    ByteSeq value(node.tokenAt(1));
                    result.push_back(INST_PUSH(size, value));
                }
            }

            result.push_back(INST_PUSH(DST_16, id));
            result.push_back(INST(IT_Jump));

            return result;
        }

        InstList toInstList_condBranchWithElse(int startIndex, InstList procWhenMatch) {
            int escapeIndex = procWhenMatch.size() + 1;
            InstList result;

            result.push_back(INST_LOAD(1));
            result.push_back(INST_JUMPIF(escapeIndex));
            result.push_back(procWhenMatch);

            return result;
        }

        InstList toInstList_condBranchWithElse(int startIndex, InstList procWhenMatch, InstList procInElse) {
            int wholeProcLen = procWhenMatch.size() + procInElse.size();
            int escapeIndex = startIndex + wholeProcLen;
            InstList result;

            result.push_back(INST_LOAD(1));
            result.push_back(INST_JUMPIFN(escapeIndex));
            result.push_back(procWhenMatch);
            result.push_back(INST_JUMP(escapeIndex));
            result.push_back(procInElse);

            return result;
        }

        InstList toInstList_loop(int startIndex, int loopCount, InstList procWhenMatch) {
            int escapeIndex = procWhenMatch.size() + 5;
            InstList result;

            result.push_back(INST_PUSH(DST_32, ByteSeq(loopCount)));
            result.push_back(INST_PUSH(DST_32, ByteSeq((int)0)));

            result.push_back(INST_LOAD(2));
            result.push_back(INST(IT_Equal));
            result.push_back(INST_LOAD(1));
            result.push_back(INST(IT_Pop));
            result.push_back(INST_JUMPIF(escapeIndex));

            result.push_back(INST_LOAD(2));
            result.push_back(INST(IT_Greater));
            result.push_back(INST_LOAD(1));
            result.push_back(INST(IT_Pop));
            result.push_back(INST_JUMPIF(escapeIndex));

            result.push_back(procWhenMatch);

            result.push_back(INST_PUSH(DST_2, ByteSeq(1)));
            result.push_back(INST(IT_Pop));
            result.push_back(INST_PUSH(DST_2, ByteSeq(1)));
            result.push_back(INST_LOAD(1));
            result.push_back(INST(IT_Add));
            result.push_back(INST_JUMPIF(startIndex + 2));

            result.push_back(INST(IT_Pop));
            result.push_back(INST(IT_Pop));

            return result;
        }

        static ByteSeq toByteSeq(Instruction inst) {
            ByteSeq instBytes;

            try {
                instBytes.push_back((Byte)inst.opcode);

                for(ByteSeq operand : inst.operand)
                    instBytes.push_back(operand);

            } catch(std::out_of_range ignored) {
                std::cout << "EXCEPTION" << std::endl;
            }

            return instBytes;
        }

        static Instruction toInst(ches::ByteSeq bytes) {
            Instruction result;

            try {
                if(bytes.size() == 0)
                    bytes = { IT_Unknown };

                result.opcode = bytes.at(0);
                vector_ext<ByteSeq> operand;

                switch(result.opcode) {
                    case IT_Block: {
                        operand.push_back(bytes.copy(1, 16));
                        operand.push_back(bytes.copy(17));
                    } break;

                    case IT_Jump:
                    case IT_JumpIf:
                    case IT_JumpIfNot: {
                        operand.push_back(bytes.copy(1));
                    } break;

                    case IT_Load: {
                        operand.push_back(bytes.copy(1));
                    } break;

                    case IT_Push: {
                        operand.push_back(bytes.copy(1, 1));
                        operand.push_back(bytes.copy(2));
                    } break;
                }

                result.operand = operand;

            } catch(std::out_of_range ignored) {
                std::cout << "EXCEPTION" << std::endl;
            }

            return result;
        }

        static std::string toString(Instruction inst) {
            // Console::writeln(instTypeMap.at(inst.opcode));
            std::string result = instTypeMap.at(inst.opcode);

            switch(inst.opcode) {
                case IT_Block: {
                    ByteSeq id = inst.operand.at(0);
                    ByteSeq name = inst.operand.at(1);

                    result += "\t";
                    result += id.toHexString();
                    result += "\t";
                    result += name.toString();
                } break;

                case IT_Jump:
                case IT_JumpIf:
                case IT_JumpIfNot: {
                    ByteSeq index = inst.operand.at(0);

                    result += "\t";
                    result += index.toHexString();
                } break;

                case IT_Load: {
                    ByteSeq len = inst.operand.at(0);

                    result += "\t";
                    result += len.toHexString();
                } break;

                case IT_Push: {
                    // IT_VarPref のエスケープを逆変換

                    std::string prefix = "";
                    // ByteSeq index;
                    ByteSeq size = inst.operand.at(0);
                    ByteSeq value = inst.operand.at(1);

                    result += "\t";
                    result += size.toHexString();
                    result += "\t";

                    // if(value.at(0) == IT_VarPref && (
                    //         value.size() <= 1 ||
                    //         value.at(1) != IT_VarPref)) {
                    //     prefix = "$";
                    //     index = value.copy(1);
                    // } else {
                    //     index = value;
                    // }

                    result += "\t";
                    result += prefix + value.toHexString();
                } break;
            }

            return result;
        }

        InstList toLSPushInstList(Node parent, int &index);

        static ValueType toValueType(Token token) {
            switch(token.type) {
                case TK_Number:
                return token.match(".") ? VT_Decimal : VT_Int;

                case TK_String:
                return VT_String;

                case TK_Char:
                return VT_Char;

                case TK_Keyword:
                if(token.match("true|false"))
                    return VT_Boolean;
                break;
            }

            return VT_Unknown;
        }

        static std::string toValueTypeName(Token token) {
            ValueType type = InstConv::toValueType(token);
            return valueTypeMap.at(type);
        }
    };
}
