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
        CPEGException_ChoiceHasNoRuleItem,
        CPEGException_FailedToParseCPEG,
        CPEGException_InvalidCPEGRuleName,
        CPEGException_InvalidCPEGSyntax,
        CPEGException_InvalidCPEGTokensIndex
    };


    class CPEGException {
    public:
        CPEGExceptionType type;
        std::unordered_map<std::string, std::string> detailMap;

        CPEGException() noexcept;

        CPEGException(CPEGExceptionType type) noexcept;

        CPEGException(CPEGExceptionType type, std::unordered_map<std::string, std::string> detailMap) noexcept;
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

        CPEGExpression() noexcept;
    };


    /*
     * spec: 連接の並びをもつ
     */
    struct CPEGExpressionSequence {
    public:
        std::vector<CPEGExpression> exprs;

        CPEGExpressionSequence() noexcept;
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
        std::vector<CPEGExpressionSequence> exprSeq;
        CPEGExpressionSequenceLoopType loopType;
        CPEGExpressionSequenceLookbehindType lookbehindType;

        CPEGExpressionSequenceGroup() noexcept;
    };


    /*
     * spec: 選択の並びをもつ
     */
    struct CPEGExpressionChoice {
    public:
        std::vector<CPEGExpressionSequenceGroup> exprSeqGroups;

        CPEGExpressionChoice() noexcept;
    };


    struct CPEGRule {
    public:
        std::string name = "";
        std::vector<CPEGExpressionChoice> exprChoices;

        CPEGRule() noexcept;
    };


    struct CPEGTokenMatchRule {
    public:
        std::string token = "";
        bool optional = false;

        CPEGTokenMatchRule(std::string token, bool optional = false) noexcept;
    };


    /*
     * spec: CPEG トークン列のインデックス ( begin / length ) をもつ
     */
    struct CPEGTokensIndex {
    private:
        std::vector<std::string> *_tokens;

        unsigned int _begin = 0;
        unsigned int _length = 0;

    public:
        /*
         * note: 全体分の範囲を表すインデックスが代入される
         * except: CPEGTokensIndex::initMembers(std::vector<std::string>*, unsigned int, unsigned int)
         */
        CPEGTokensIndex(std::vector<std::string> *tokens);

        /*
         * note: begin から最後までの範囲を表すインデックスが代入される
         * except: CPEGTokensIndex::initMembers(std::vector<std::string>*, unsigned int, unsigned int)
         */
        CPEGTokensIndex(std::vector<std::string> *tokens, unsigned int begin);

        /*
         * note: begin から length 分の範囲を表すインデックスが代入される
         * except: CPEGTokensIndex::initMembers(std::vector<std::string>*, unsigned int, unsigned int)
         */
        CPEGTokensIndex(std::vector<std::string> *tokens, unsigned int begin, unsigned int length);

    private:
        /*
         * except: CPEGException [InvalidCPEGTokensIndex]
         */
        void initMembers(std::vector<std::string> *tokens, unsigned int begin, unsigned int length);

    public:
        inline unsigned int begin() noexcept {
            return this->_begin;
        }

        inline unsigned int end() noexcept {
            return this->_begin + this->_length;
        }

        inline unsigned int length() noexcept {
            return this->_length;
        }

        inline std::vector<std::string>* tokens() noexcept {
            return this->_tokens;
        }
    };


    class CPEG {
    public:
        std::vector<CPEGRule> rules;

        static std::regex idTokenRegex;
        static std::regex symbolTokenRegex;
        static std::regex spacingTokenRegex;

        CPEG() noexcept;

        /*
         * excep: 
         * note: 事前に CPEG::loadCPEGFile(std::string) を実行すること
         */
        SyntaxTree getSyntaxTree(std::string &source);

        /*
         * excep: FileManager::getLines(std::string) / getCPEGRule(std::string, CPEGRule)
         */
        void loadCPEGFile(std::string filePath);

        /*
         * excep: 
         */
        static std::vector<CPEGExpressionChoice> getCPEGExpressionChoices(CPEGTokensIndex tokensIndex) {
            std::vector<CPEGExpressionChoice> choices;

            std::vector<std::string> *tokens = tokensIndex.tokens();

            unsigned int begin = tokensIndex.begin();
            unsigned int end = tokensIndex.end();

            unsigned int seqGroupBegin = begin;

            try {
                for(int i = begin; i <= end; i++) {
                    if(i == end || tokens->at(i) == ">") {
                        int seqGroupEnd = i;

                        CPEGTokensIndex groupTokensIndex(tokens, seqGroupBegin, seqGroupEnd - seqGroupBegin);

                        std::vector<CPEGExpressionSequenceGroup> group = CPEG::getCPEGExpressionSequenceGroups(groupTokensIndex);

                        CPEGExpressionChoice newChoice;
                        newChoice.exprSeqGroups = group;

                        choices.push_back(newChoice);

                        seqGroupBegin = seqGroupEnd + 1;
                    }
                }
                std::cout<<std::endl;
            } catch(std::out_of_range excep) {
                throw excep;
            } catch(CPEGException excep) {
                throw excep;
            }

            return choices;
        }

        /*
         * excep: 
         */
        static std::vector<CPEGExpressionSequenceGroup> getCPEGExpressionSequenceGroups(CPEGTokensIndex tokensIndex) {
            if(CPEG::isCPEGTokensEmpty(tokensIndex))
                throw CPEGException(CPEGException_ChoiceHasNoRuleItem);

            std::vector<CPEGExpressionSequenceGroup> groups;

            std::vector<std::string> *tokens = tokensIndex.tokens();

            unsigned int begin = tokensIndex.begin();
            unsigned int end = tokensIndex.end();

            for(int j = begin; j < end; j++) std::cout << tokens->at(j) << "|"; std::cout << std::endl;

            try {
                for(int i = begin; i < end; i++) {
                    // if(tokens.at(i) == '(') {
                    //     CPEGExpressionSequence = CPEG::getCPEGExpressionSequence();
                    // }
                }
            } catch(std::out_of_range excep) {
                throw excep;
            }

            return groups;
        }

        /*
         * arg: line: 解析させる行 ( 値の変更なし ); rule: 行の解析後に代入される規則値 ( 規則文であった場合のみ値が変更される )
         * ret: 与えられた行が規則文であるか
         * excep: CPEGException [InvalidCPEGSyntax, InvalidCPEGRuleName] / getCPEGTokens(std::string&)
         */
        static bool getCPEGRule(std::string &line, CPEGRule &rule) {
            CPEGRule newRule;
            std::vector<std::string> tokens = CPEG::getCPEGTokens(line);

            if(tokens.size() == 0)
                return false;

            if(tokens.at(0) == "#")
                return false;

            if(tokens.size() < 4)
                throw CPEGException(CPEGException_InvalidCPEGSyntax);

            int ruleDefStartIndex = 0;

            if(CPEG::matchCPEGTokens(tokens, { "", ":", "=" }, ruleDefStartIndex, true)) {
                if(!std::regex_match(tokens.at(0), CPEG::idTokenRegex))
                    throw CPEGException(CPEGException_InvalidCPEGRuleName);

                newRule.name = tokens.at(0);
                rule.exprChoices = CPEG::getCPEGExpressionChoices(CPEGTokensIndex(&tokens, ruleDefStartIndex, tokens.size() - ruleDefStartIndex));
            }

            // newRule.exprChoices.push_back();

            rule = newRule;

            return true;
        }

        /*
         * excep: CPEGException [FailedToParseCPEG]
         */
        static std::vector<std::string> getCPEGTokens(std::string &line) {
            std::vector<std::string> tokens;
            std::string tmpToken = "";

            bool inString = false;

            try {
                for(int i = 0; i < line.size(); i++) {
                    if(line.at(i) == '"') {
                        tmpToken.push_back(line.at(i));

                        for(i++; i < line.size(); i++) {
                            tmpToken.push_back(line.at(i));

                            if(line.at(i) == '"') {
                                tokens.push_back(tmpToken);
                                break;
                            }
                        }

                        tmpToken = "";

                        continue;
                    }

                    if(line.at(i) == '#') {
                        tokens.push_back({ line.at(i) });

                        tmpToken = "";

                        break;
                    }

                    if(std::regex_match(std::string { line.at(i) }, CPEG::symbolTokenRegex)) {
                        if(tmpToken != "")
                            tokens.push_back(tmpToken);

                        tmpToken = "";

                        tokens.push_back({ line.at(i) });
                        tmpToken = "";

                        continue;
                    }
                    
                    if(std::regex_match(std::string { line.at(i) }, CPEG::spacingTokenRegex)) {
                        if(tmpToken != "")
                            tokens.push_back(tmpToken);

                        tmpToken = "";

                        tokens.push_back(" ");

                        for(int j = i + 1; j < line.size(); j++) {
                            if(!std::regex_match(std::string { line.at(j) }, CPEG::spacingTokenRegex)) {
                                i = j - 1;
                                break;
                            }
                        }

                        continue;
                    }

                    if(std::regex_match(std::string { line.at(i) }, CPEG::idTokenRegex)) {
                        tmpToken.push_back(line.at(i));
                        continue;
                    }

                    std::cout << line.at(i) << std::endl;

                    throw CPEGException(CPEGException_InvalidCPEGSyntax);
                }
            } catch(std::regex_error excep) {
                throw CPEGException(CPEGException_FailedToParseCPEG);
            }

            if(tmpToken != "")
                tokens.push_back(tmpToken);

            return tokens;
        }

        static bool isCPEGTokensEmpty(CPEGTokensIndex tokensIndex) noexcept {
            std::vector<std::string> *tokens = tokensIndex.tokens();

            unsigned int begin = tokensIndex.begin();
            unsigned int end = tokensIndex.end();

            for(int i = begin; i < end; i++)
                if(tokens->at(i) != "" && tokens->at(i) != " ")
                    return false;

            return true;
        }

        static bool matchCPEGTokens(std::vector<std::string> tokens, std::vector<std::string> tokenMatchRules, int &matchedTokenLen, bool ignoreSpacing = false) noexcept {
            int matchRuleIndex = 0;
            int tokenIndex = 0;

            for(; tokenIndex < tokens.size(); tokenIndex++) {
                const std::string checkToken = tokens.at(tokenIndex);

                if(matchRuleIndex >= tokenMatchRules.size())
                    break;

                if(ignoreSpacing && checkToken == " ")
                    continue;

                std::string rule = tokenMatchRules.at(matchRuleIndex);
                matchRuleIndex++;

                if(checkToken != rule) {
                    if(rule == "")
                        continue;

                    return false;
                }
            }

            matchedTokenLen = tokenIndex;

            return true;
        }
    };
}
