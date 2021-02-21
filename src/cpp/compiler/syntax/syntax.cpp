/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
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

        SyntaxTreeNode();
    };


    struct SyntaxTree {
    public:
        SyntaxTreeNode root;

        SyntaxTree();
    };


    enum CPEGExpressionType {
        CPEGExpression_Unknown,
        CPEGExpression_ID,
        CPEGExpression_Regex,
        CPEGExpression_String
    };


    struct CPEGExpression {
    public:
        CPEGExpressionType type = CPEGExpression_Unknown;
        std::string value;
    };


    /*
     * spec: 連接の並びをもつ
     */
    struct CPEGExpressionSequence {
    public:
        std::vector<CPEGExpression> exprs;

        CPEGExpressionSequence();
    };


    enum CPEGExpressionSequenceLoopType {
        CPEGExpressionSequenceLoop_ZeroOrMore,
        CPEGExpressionSequenceLoop_OneOrMore,
        CPEGExpressionSequenceLoop_ZeroOrOne,
    };


    enum CPEGExpressionSequenceLookbehindType {
        CPEGExpressionSequenceLookbehind_PositiveLookbehind,
        CPEGExpressionSequenceLookbehind_NegativeLookbehind
    };


    /*
     * spec: 連接グループの並びをもつ; グループ化されてない場合も要素数 1 として使用する
     */
    struct CPEGExpressionSequenceGroup {
    public:
        std::vector<CPEGExpressionSequence> exprs;
        CPEGExpressionSequenceLoopType loopType;
        CPEGExpressionSequenceLookbehindType lookbehindType;

        CPEGExpressionSequenceGroup();
    };


    /*
     * spec: 選択の並びをもつ
     */
    struct CPEGExpressionChoice {
    public:
        std::vector<CPEGExpressionSequenceGroup> seqs;

        CPEGExpressionChoice();
    };


    struct CPEGRule {
    public:
        std::string name = "";
        std::vector<CPEGExpressionChoice> exprChoices;

        CPEGRule(std::string name);
    };


    class CPEG {
    public:
        std::vector<CPEGRule> rules;

        CPEG();
    };
}
