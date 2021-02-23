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
    enum CPEGExceptionType {
        CPEGException_Unknown,
        CPEGException_InvalidCPEGSyntax,
    };


    class CPEGException {
    public:
        CPEGExceptionType type;
        std::unordered_map<std::string, std::string> detailMap;

        CPEGException();

        CPEGException(CPEGExceptionType type);

        CPEGException(CPEGExceptionType type, std::unordered_map<std::string, std::string> detailMap);
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

        CPEGExpression();
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

        CPEGRule();
    };


    class CPEG {
    public:
        std::vector<CPEGRule> rules;

        CPEG();

        SyntaxTree getSyntaxTree(std::string &source);

        /*
         * excep: FileManager::getLines(std::string) と同様
         */
        void loadCPEGFile(std::string filePath);

        /*
         * excep: CPEGException [InvalidCPEGSyntax]
         */
        static CPEGRule getCPEGRule(std::string &line) {
            CPEGRule rule;
            std::vector<std::string> tokens = CPEG::getCPEGTokens(line);

            if(tokens.size() < 4)
                throw CPEGException(CPEGException_InvalidCPEGSyntax);

            // rule.name = "";
            // rule.exprChoices.push_back();

            return rule;
        }

        static std::vector<std::string> getCPEGTokens(std::string &line) {
            std::vector<std::string> tokens;
            std::string tmpToken = "";

            std::regex symbolRegex("[.*+?&!:=]");
            std::regex spacingRegex("[ \t]");

            bool inString = false;

            for(int i = 0; i < line.size(); i++) {
                if(std::regex_match(&(line.at(i)), symbolRegex)) {
                    tokens.push_back(std::to_string(line.at(i)));
                    tmpToken = "";
                } else if(std::regex_match(&(line.at(i)), spacingRegex)) {
                    tokens.push_back(" ");

                    for(i++; i < line.size(); i++) {
                        if(!std::regex_match(&(line.at(i)), spacingRegex)) {
                            i--;
                            break;
                        }
                    }
                } else if(line.at(i) == '"') {
                    tmpToken.push_back(line.at(i));

                    for(i++; i < line.size(); i++) {
                        tmpToken.push_back(line.at(i));

                        if(line.at(i) == '"') {
                            tokens.push_back(tmpToken);
                            continue;
                        }
                    }

                    tmpToken = "";
                } else {
                    tmpToken.push_back(line.at(i));
                }
            }

            if(tmpToken != "")
                tokens.push_back(tmpToken);

            return tokens;
        }
    };
}
