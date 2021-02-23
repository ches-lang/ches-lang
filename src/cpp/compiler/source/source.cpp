/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


namespace ches::compiler {
    struct SourceFile {
    public:
        std::string filePath = "";
        std::string source = "";
        SyntaxTree syntaxTree;

        /*
         * excep: SourceFile::loadSourceFile() と同様
         */
        SourceFile(std::string filePath);

    private:
        /*
         * excep: SourceFile::getSyntaxTree() と同様 / SourceFile::getSource() と同様
         */
        void loadSourceFile();

        /*
         * FileManager::getText(std::string) と同様
         */
        std::string getSource();

        SyntaxTree getSyntaxTree();
    };
}
