/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


namespace ches::compiler {
    struct SyntaxTreeNode {
    public:
        std::string name = "";
        std::vector<SyntaxTreeNode> nodes;
        std::vector<std::string> tokens;

        SyntaxTreeNode() noexcept;
    };


    struct SyntaxTree {
    public:
        SyntaxTreeNode root;

        SyntaxTree() noexcept;
    };
}
