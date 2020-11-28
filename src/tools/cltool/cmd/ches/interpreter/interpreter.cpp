#pragma once


namespace ches {
    struct Label {
        ByteVec id;
        ByteVec name;

        int beginIndex;
        int endIndex;

        Label();

        Label(ByteVec id, ByteVec name, int beginIndex, int endIndex);
    };


    class Interpreter {
    public:
        // HeaderInfo headerInfo;
        ByteVec magicNum;

        ByteVec header;
        ByteVec body;

        std::vector<ByteVec> instList;

        Interpreter(std::string filePath);

        void runProgram();

    private:
        int index = 0;
        std::stack<Stack> stack;
        std::stack<Stack> opeStack;
        // <id, label>
        std::map<ByteVec, Label> labelList;

        std::vector<ByteVec> divideInsts();

        static std::string instToString(ByteVec inst) {
            std::string result;

            try {
                Byte opcode = inst.at(0);
                result = instTypeMap.at(opcode);

                switch(opcode) {
                    case IT_Block: {
                        ByteVec id;
                        ByteVec name;

                        std::copy(inst.begin() + 1, inst.end() - inst.size() + 17, std::back_inserter(id));
                        std::copy(inst.begin() + 18, inst.end(), std::back_inserter(name));

                        result += "\t";
                        result += Interpreter::toHexString(id);
                        result += "\t";
                        result += Interpreter::toHexString(name);
                    } break;

                    case IT_Jump:
                    case IT_JumpIf:
                    case IT_JumpIfNot: {
                            ByteVec index;
                        std::copy(inst.begin() + 1, inst.end(), std::back_inserter(index));

                        result += "\t";
                        result += Interpreter::toHexString(index);
                    } break;

                    case IT_Load: {
                        ByteVec len;
                        std::copy(inst.begin() + 1, inst.end(), std::back_inserter(len));

                        result += "\t";
                        result += Interpreter::toHexString(len);
                    } break;

                    case IT_Push: {
                        // IT_VarPref のエスケープを逆変換

                        std::string prefix = "";
                        // ByteSeq index;
                        ByteVec size;
                        ByteVec value;

                        std::copy(inst.begin() + 1, inst.end() - inst.size() + 2, std::back_inserter(size));
                        std::copy(inst.begin() + 2, inst.end(), std::back_inserter(value));

                        result += "\t";
                        result += Interpreter::toHexString(size);
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
                        result += prefix + Interpreter::toHexString(value);
                    } break;
                }
            } catch(std::out_of_range ignored) {
                std::cout << "EXCEPTION" << std::endl;
            }

            return result;
        }

        void loadCompiledFile(std::string filePath);

        void printStackLog();

        void setLabelData();

        void runNextInst();

        std::string joinCode(ByteVec src);

        static std::string toHexString(ByteVec source, std::string separator = "") {
            std::string result;

            for(int src : source) {
                std::stringstream ss;
                ss << std::hex << src;
                result += (src < 16 ? "0" : "") + ss.str() + separator;
            }

            if(source.size() > 0)
                for(int i = 0; i < separator.length(); i++)
                    result.pop_back();

            return result;
        }
    };
}
