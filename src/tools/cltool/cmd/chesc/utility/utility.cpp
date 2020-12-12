#pragma once


namespace ches {
    struct ByteSeq;
    struct LineSeq;
    struct FuncList;


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

        inline void fill(T filler, int tarSize) {
            this->push_back(filler, tarSize - this->size());
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

        ByteSeq(long value);

        ByteSeq(std::string source);

        ByteSeq(Token token);

        ByteSeq(Node node);

        // vector_ext<T>::copy() のカプセル化 (セグフォ防止のため)

        ByteSeq copy(int begin);

        ByteSeq copy(int begin, int end);

        ByteSeq escape();

        inline void fill(Byte filler, int tarSize) {
            this->push_back(filler, tarSize - this->size());
        }

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

        inline void push_back(Byte value, int length) {
            for(int i = 0; i < length; i++)
                vector_ext<Byte>::push_back(value);
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

        Byte* toPointerArray();

        std::string toString();
    };


    struct LineSeq : public vector_ext<ByteSeq> {
        LineSeq();

        LineSeq(ByteSeq value);

        LineSeq(std::initializer_list<ByteSeq> value);

        LineSeq(std::vector<ByteSeq> value);
    };


    struct Inst {
        Byte opcode = IT_Unknown;
        std::vector<ByteSeq> operand;

        Inst();

        Inst(int opcode);

        Inst(int opcode, std::vector<ByteSeq> operand);
    };


    struct InstList : public vector_ext<Inst> {
        InstList();

        InstList(Inst value);

        InstList(std::initializer_list<Inst> value);

        InstList(std::vector<Inst> value);

        int byteSize();

        inline void push_back(Inst value) {
            vector_ext<Inst>::push_back(value);
        }

        inline void push_back(InstList value) {
            for(Inst val : value)
                vector_ext<Inst>::push_back(val);
        }
    };


    struct Func {
        ByteSeq id;
        ByteSeq name;
        InstList instList;
        int begin;
        int end;

        Func();

        Func(ByteSeq id, ByteSeq name);

        Func(ByteSeq id, ByteSeq name, int begin);

        Func(ByteSeq id, ByteSeq name, int begin, int end);
    };


    struct FuncList : public vector_ext<Func> {
        FuncList();

        FuncList(Func value);

        FuncList(std::initializer_list<Func> value);

        FuncList(std::vector<Func> value);

        bool existsId(ByteSeq id);

        bool existsName(ByteSeq name);

        Func findById(ByteSeq id);

        Func findByName(ByteSeq name);

        inline void pop_back() {
            vector_ext<Func>::pop_back();
        }

        inline void pop_back(int len) {
            for(int i = 0; i < len; i++)
                vector_ext<Func>::pop_back();
        }

        inline void push_back() {
            vector_ext<Func>::push_back(Func());
        }

        inline void push_back(Func value) {
            vector_ext<Func>::push_back(value);
        }

        inline void push_back(std::vector<Func> value) {
            for(Func val : value)
                vector_ext<Func>::push_back(val);
        }

        inline void push_back(vector_ext<Func> value) {
            for(Func val : value)
                vector_ext<Func>::push_back(val);
        }

        inline void push_front(Func value) {
            vector_ext<Func>::insert(this->begin(), value);
        }

        inline void push_back(FuncList value) {
            for(Func val : value)
                vector_ext<Func>::push_back(val);
        }
    };


    struct ByteConvData {
        ByteSeq *magicNum;
        InstList *instList;
        FuncList *funcList;

        ByteConvData();
    };


    class InstConv {
    public:
        int index = 0;
        Node tree;

        int sumByteLen = HEADER_LEN;

        std::string filePath;
        std::string source;

        FuncList blockList;
        std::string nameSpaceName;
        std::string className;

        int localStackLen = 0;
        int localListLen = 0;

        InstConv();


        static ByteSeq toByteSeq(Node tree, std::string filePath, std::string value) {
            ByteSeq result;
            // 非デバッグ時にはfalseにすること
            bool enableDebugMode = true;

            // ボディ部分

            InstConv instConv;
            InstList instList = (enableDebugMode ? instConv.toInstList_forDebugging() : instConv.toInstList(tree, filePath, value));
            ByteSeq body = InstConv::toByteSeq(instList);

            // ヘッダ部分

            ByteSeq header = InstConv::toHeaderBytes(body.size());

            // IDエリア部分

            ByteSeq idArea = InstConv::toIDAreaBytes(instConv.blockList);

            // まとめて追加

            result.push_back(header);
            result.push_back(body);
            result.push_back(idArea);

            // std::cout << std::endl;

            return result;
        }

        static std::string toString(Inst inst) {
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

    private:
        static ByteSeq toByteSeq(InstList instList) {
            ByteSeq result;

            try {
                for(Inst inst : instList) {
                    result.push_back((Byte)inst.opcode);

                    for(ByteSeq operand : inst.operand)
                        result.push_back(operand);

                    result.push_back((IT_InstDiv));
                }
            } catch(std::out_of_range ignored) {
                std::cout << "EXCEPTION" << std::endl;
            }

            return result;
        }

        static ByteSeq toHeaderBytes(int bodyByteSize) {
            ByteSeq result;

            ByteSeq magicNum = MAGIC_NUM;

            ByteSeq idAreaIndex = ByteSeq(HEADER_LEN + bodyByteSize).escape();
            idAreaIndex.push_back(IT_InstDiv);
            idAreaIndex.fill(0, 16);

            result.push_back(magicNum);
            result.push_back(idAreaIndex);
            result.fill(0, HEADER_LEN);

            return result;
        }

        static ByteSeq toIDAreaBytes(FuncList &funcList) {
            ByteSeq result;

            for(Func func : funcList) {
                result.push_back(func.id);
                result.push_back(ByteSeq(func.name).escape());
                result.push_back(IT_InstDiv);
                result.push_back(ByteSeq(func.begin).escape());
                result.push_back(IT_InstDiv);
            }

            return result;
        }

        InstList toInstList(Node tree);

        InstList toInstList(Node parent, int &index);

        InstList toInstList(Node tree, std::string filePath, std::string source);

        InstList toInstList_forDebugging();

        InstList toInstList_callFunc(Node &node) {
            InstList result;

            Token nameToken = node.tokenAt(0);
            std::string name = nameToken.string;

            ByteSeq id = this->blockList.findByName(ByteSeq(name)).id;

            if(id.size() != 16)
                Console::log(LogType_Error, 1822, { { "At", nameToken.getPositionText(this->filePath, this->source ) }, { "Id", name } }, false);

            Node args = node.childAt(0);

            for(Node node : args.children) {
                if(node.type == ND_Value) {
                    DataSizeType size = dataSizeTypeMap.at(node.tokenAt(0).string);
                    ByteSeq value(node.tokenAt(1));
                    result.push_back(INST_PUSH(size, value));
                }
            }

            result.push_back(INST_PUSH(DST_16, id));
            result.push_back(INST(IT_Jump));

            return result;
        }

        InstList toInstList_condBranchWithElse(int startIndex, InstList procWhenMatch) {
            // escapeIndexを修正中
            int escapeIndex = procWhenMatch.byteSize();
            InstList result;

            result.push_back(INST_LOAD(1));
            result.push_back(INST_JUMPIF(escapeIndex));
            result.push_back(procWhenMatch);

            return result;
        }

        InstList toInstList_condBranchWithElse(int startIndex, InstList procWhenMatch, InstList procInElse) {
            int wholeProcLen = procWhenMatch.byteSize() + procInElse.byteSize();
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
            int escapeIndex = procWhenMatch.byteSize() + 5;
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
    };
}
