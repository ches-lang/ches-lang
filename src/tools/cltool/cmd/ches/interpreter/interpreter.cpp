#pragma once


namespace ches::run {
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

            if(separator.length() && separator.length() >= result.length())
                for(int i = 0; i < separator.length(); i++)
                    result.pop_back();

            return result;
        }
    };
}
