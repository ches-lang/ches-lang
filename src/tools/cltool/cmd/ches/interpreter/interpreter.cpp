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
