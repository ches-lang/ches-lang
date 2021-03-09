/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


namespace ches::compiler {
    struct SourceFile {
    public:
        std::string filePath = "";
        std::string source = "";
        SyntaxTree syntaxTree;

        SourceFile(std::string filePath) noexcept;

        /*
         * excep: SourceFile::getSyntaxTree() / SourceFile::getSource()
         */
        void loadSourceFile(CPEG *cpeg);
    };
}
