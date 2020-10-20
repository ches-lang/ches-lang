#pragma once


namespace ches {
    #define HEADER_LEN      128
    #define MAGIC_NUMBER    ByteSeq { 0x4F, 0x52, 0x49, 0x4E, 0x43, 0x48, 0x41, 0x4E }

    typedef unsigned char   Byte;

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
        TK_RightBrace,
    };


    enum NodeType : Byte {
        ND_Unknown,
        ND_Root,
        ND_Token,
        ND_DefVar,
        ND_InitVar,
        ND_DefFunc,
        ND_CallFunc,
        ND_Args,
        ND_If,
        ND_Else,
        ND_ElseIf,
        ND_Loop,
        ND_Loop_Cond,
        ND_Loop_Init,
        ND_Loop_Change,
        ND_Count,
        ND_Logic,
        ND_Compare,
        ND_Expression,
        ND_Operator,
        ND_Equal,
        ND_Less,
        ND_Greater,
        ND_LessEqual,
        ND_GreaterEqual,
    };


    enum InstType : Byte {
        IT_Unknown,
        IT_LineDiv,
        IT_TokenDiv,
        IT_VarPref,
        IT_LineIndex,
        IT_Label,
        IT_LSPush,
        IT_LSPop,
        IT_LLPush,
        IT_Logic_And,
        IT_Logic_Or,
        IT_Compare,
        IT_Calc_Add,
        IT_Calc_Sub,
        IT_Calc_Multi,
        IT_Calc_Div,
        IT_Jump,
        IT_IFJump,
    };


    // tokenTypeMapを追加する


    std::unordered_map<Byte, std::string> nodeTypeMap {
        { ND_Unknown, "Unknown" },
        { ND_Root, "Root" },
        { ND_Token, "Token" },
        { ND_DefVar, "DefVar" },
        { ND_InitVar, "InitVar" },
        { ND_DefFunc, "DefFunc" },
        { ND_CallFunc, "CallFunc" },
        { ND_Args, "Args" },
        { ND_If, "If" },
        { ND_Else, "Else" },
        { ND_ElseIf, "ElseIf" },
        { ND_Loop, "Loop" },
        { ND_Loop_Cond, "Loop_Cond" },
        { ND_Loop_Init, "Loop_Init" },
        { ND_Loop_Change, "Loop_Change" },
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

        ByteSeq(Byte value, int len);

        ByteSeq(std::initializer_list<Byte> value);

        ByteSeq(int source);

        ByteSeq(std::string source);

        ByteSeq(Token token);

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

        static ByteSeq join(vector_ext<ByteSeq> byteSeqs, ByteSeq separator) {
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

        std::string toHexString(std::string sep = "");

        int toInt();

        LineSeq toLineSeq();

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
        std::unordered_map<std::string, ByteSeq> operand;

        Instruction();

        Instruction(ByteSeq bytes);

        Instruction(int opcode);

        Instruction(int opcode, std::unordered_map<std::string, ByteSeq> operand);
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


    class InstConv {
    public:
        //InstList instList;
        int index;
        Node tree;

        std::string filePath;
        std::string source;

        LineSeq *lines;
        FuncList *labelList;
        std::string nameSpaceName;
        std::string className;

        int localStackLen = 0;
        int localListLen = 0;

        InstConv();

        InstList toInstList(Node tree);

        InstList toInstList(Node parent, int &index);

        InstList toInstList(Node tree, std::string filePath, std::string source);

        static ByteSeq toByteSeq(Instruction inst) {
            ByteSeq instBytes;

            try {
                switch(inst.opcode) {
                    case IT_Unknown: {
                        instBytes.push_back((Byte)IT_Unknown);
                    } break;

                    case IT_Label: {
                        instBytes.push_back((Byte)IT_Label);
                        instBytes.push_back(inst.operand["id"]);
                        instBytes.push_back(inst.operand["name"].escape());
                    } break;

                    case IT_LSPush: {
                        instBytes.push_back((Byte)IT_LSPush);
                        instBytes.push_back(inst.operand["index"].escape());
                    } break;

                    case IT_LSPop: {
                        instBytes.push_back((Byte)IT_LSPop);
                    } break;

                    case IT_LLPush: {
                        instBytes.push_back((Byte)IT_LLPush);
                        instBytes.push_back(inst.operand["index"].escape());
                    } break;

                    case IT_Compare: {
                        instBytes.push_back((Byte)IT_Compare);
                    } break;

                    case IT_Jump: {
                        instBytes.push_back((Byte)IT_Jump);
                        instBytes.push_back(inst.operand["index"].escape());
                    } break;

                    case IT_IFJump: {
                        instBytes.push_back((Byte)IT_IFJump);
                        instBytes.push_back(inst.operand["index"].escape());
                    } break;

                    default: {
                        instBytes.push_back((Byte)IT_Unknown);
                    } break;
                }
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

                result.opcode = bytes[0];

                switch(result.opcode) {
                    case IT_Label: {
                        result.operand["id"] = bytes.copy(1, 16);
                        result.operand["name"] = bytes.copy(17 , -1);
                    } break;

                    case IT_LSPush: {
                        result.operand["index"] = bytes.copy(1, -1);
                    } break;

                    case IT_LSPop:
                    break;

                    case IT_LLPush: {
                        result.operand["index"] = bytes.copy(1, -1);
                    } break;

                    case IT_Compare:
                    break;

                    case IT_Jump: {
                        result.operand["index"] = bytes.copy(1, -1);
                    } break;

                    case IT_IFJump: {
                        result.operand["index"] = bytes.copy(1, -1);
                    } break;
                }
            } catch(std::out_of_range ignored) {
                std::cout << "EXCEPTION" << std::endl;
            }

            return result;
        }

        static std::string toString(Instruction inst) {
            std::string text = "";

            switch(inst.opcode) {
                case IT_Unknown:
                return "; Unknown";

                case IT_Label:
                return "label\t" + inst.operand["id"].toHexString() + "\t" + ByteSeq(inst.operand["name"]).toString();

                case IT_LSPush: {
                    // IT_VarPref のエスケープを逆変換

                    std::string prefix = "";
                    ByteSeq index;

                    if(inst.operand["index"].at(0) == IT_VarPref && (
                        inst.operand["index"].size() <= 1 ||
                        inst.operand["index"].at(1) != IT_VarPref)) {
                            prefix = "$";
                            index = inst.operand["index"].copy(1);
                    } else {
                        index = inst.operand["index"];
                    }

                    return "lspush\t" + prefix + index.toHexString();
                }

                case IT_LSPop:
                return "lspop";

                case IT_LLPush: {
                    // IT_VarPref のエスケープを逆変換

                    std::string prefix = "";
                    ByteSeq index;

                    if(inst.operand["index"].at(0) == IT_VarPref && (
                        inst.operand["index"].size() < 2 ||
                        inst.operand["index"].at(1) != IT_VarPref)) {
                            prefix = "$";
                            index = inst.operand["index"].copy(1);
                    } else {
                        index = inst.operand["index"];
                    }

                    return "llpush\t" + prefix + index.toHexString();
                }

                case IT_Compare:
                return "comp";

                case IT_Jump:
                return "jump\t" + inst.operand["index"].toHexString();

                case IT_IFJump: {
                    // IT_VarPref のエスケープを逆変換

                    std::string prefix = "";
                    ByteSeq index;

                    if(inst.operand["index"].at(0) == IT_VarPref && (
                        inst.operand["index"].size() <= 1 ||
                        inst.operand["index"].at(1) != IT_VarPref)) {
                            prefix = "$";
                            index = inst.operand["index"].copy(1);
                    } else {
                        index = inst.operand["index"];
                    }

                    return "ifjump\t" + prefix + std::to_string(index.toInt());
                }

                default:
                return "; Unknown_";
            }

            return text;
        }
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

        Function findById(ByteSeq id);

        Function findByName(ByteSeq name);

        inline void push_back(Function value) {
                vector_ext<Function>::push_back(value);
        }

        inline void push_back(FuncList value) {
            for(Function val : value)
                vector_ext<Function>::push_back(val);
        }
    };


    struct Stack : public std::stack<void*> {
        Stack();
    };


    struct StackList : vector_ext<Stack> {
        StackList();
    };


    struct HeaderInfo {
        ByteSeq magicNum;

        HeaderInfo();

        HeaderInfo(ByteSeq bytes);

        ByteSeq toByteSeq();
    };
}
