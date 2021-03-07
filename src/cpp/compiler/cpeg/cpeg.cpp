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
    enum CPEGExceptionType {
        CPEGException_Unknown,
        CPEGException_InvalidSequenceGroupParen,
        CPEGException_ChoiceHasNoExpression,
        CPEGException_FailedToParseCPEG,
        CPEGException_InvalidCPEGRuleName,
        CPEGException_InvalidCPEGSyntax,
        CPEGException_InvalidCPEGTokensIndex,
        CPEGException_SequenceGroupHasNoExpression
    };


    class CPEGException {
    public:
        CPEGExceptionType type;
        std::unordered_map<std::string, std::string> detailMap;

        CPEGException() noexcept;

        CPEGException(CPEGExceptionType type) noexcept;

        CPEGException(CPEGExceptionType type, std::unordered_map<std::string, std::string> detailMap) noexcept;
    };


    enum CPEGExpressionLoopType {
        CPEGExpressionLoop_Default,
        CPEGExpressionLoop_ZeroOrMore,
        CPEGExpressionLoop_OneOrMore,
        CPEGExpressionLoop_ZeroOrOne,
    };


    enum CPEGExpressionLookbehindType {
        CPEGExpressionLookbehind_Default,
        CPEGExpressionLookbehind_PositiveLookbehind,
        CPEGExpressionLookbehind_NegativeLookbehind
    };


    struct CPEGExpressionProperties {
    public:
        CPEGExpressionLoopType loopType = CPEGExpressionLoop_Default;
        CPEGExpressionLookbehindType lookbehindType = CPEGExpressionLookbehind_Default;

        CPEGExpressionProperties() noexcept;

        static CPEGExpressionLoopType toLoopType(char token) noexcept {
            switch(token) {
                case '*':
                return CPEGExpressionLoop_ZeroOrMore;

                case '+':
                return CPEGExpressionLoop_OneOrMore;

                case '?':
                return CPEGExpressionLoop_ZeroOrOne;
            }

            return CPEGExpressionLoop_Default;
        }

        static CPEGExpressionLookbehindType toLookbehindType(char token) noexcept {
            switch(token) {
                case '&':
                return CPEGExpressionLookbehind_PositiveLookbehind;

                case '!':
                return CPEGExpressionLookbehind_NegativeLookbehind;
            }

            return CPEGExpressionLookbehind_Default;
        }
    };


    enum CPEGExpressionType {
        CPEGExpression_Unknown,
        CPEGExpression_CharClass,
        CPEGExpression_ID,
        CPEGExpression_String,
        CPEGExpression_WildCard
    };


    struct CPEGExpression {
    public:
        CPEGExpressionType type = CPEGExpression_Unknown;
        std::string value;

        CPEGExpressionProperties props;

        CPEGExpression() noexcept;
    };


    /*
     * spec: 連接の並びをもつ
     */
    struct CPEGExpressionSequence {
    public:
        std::vector<CPEGExpression> exprs;

        CPEGExpressionProperties props;

        CPEGExpressionSequence() noexcept;
    };


    /*
     * spec: 連接グループの並びをもつ; グループ化されてない場合も要素数 1 として使用する
     */
    struct CPEGExpressionSequenceGroup {
    public:
        std::vector<CPEGExpressionSequence> exprSeqs;

        CPEGExpressionSequenceGroup() noexcept;
    };


    /*
     * spec: 選択の並びをもつ
     */
    struct CPEGExpressionChoice {
    public:
        CPEGExpressionSequenceGroup exprSeqGroup;

        CPEGExpressionChoice() noexcept;
    };


    struct CPEGRule {
    public:
        std::string name = "";
        std::vector<CPEGExpressionChoice> exprChoices;

        CPEGRule() noexcept;
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

        static std::regex charClassTokenRegex;
        static std::regex idTokenRegex;
        static std::regex spacingTokenRegex;
        static std::regex stringTokenRegex;
        static std::regex symbolTokenRegex;

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
         * ret: expr が空でないかどうか
         * arg: expr: expr が空でない場合のみ変換後の expression を代入する
         * excep: getCPEGExpression(std::string)
         */
        static bool getCPEGExpression(CPEGTokensIndex choiceTokensIndex, CPEGExpression &expr) {
            if(CPEG::isCPEGTokensEmpty(choiceTokensIndex))
                return false;

            std::vector<std::string> *tokens = choiceTokensIndex.tokens();

            unsigned int begin = choiceTokensIndex.begin();
            unsigned int end = choiceTokensIndex.end();

            std::string tmpToken = "";

            for(int i = begin; i < end; i++)
                tmpToken += tokens->at(i);

            try {
                expr = CPEG::getCPEGExpression(tmpToken);
            } catch(std::regex_error excep) {
                throw excep;
            } catch(CPEGException excep) {
                throw excep;
            }

            return true;
        }

        /*
         * excep: std::regex_error / CPEGException [InvalidCPEGSyntax]
         */
        static CPEGExpression getCPEGExpression(std::string token) {
            CPEGExpression expr;
            CPEGExpressionProperties props = CPEG::getCPEGExpressionProperties(token);

            if(std::regex_match(token, CPEG::charClassTokenRegex)) {
                expr.type = CPEGExpression_CharClass;
                expr.value = token.substr(1, token.size() - 2);
                expr.props = props;

                return expr;
            }

            if(std::regex_match(token, CPEG::idTokenRegex)) {
                expr.type = CPEGExpression_ID;
                expr.value = token;
                expr.props = props;

                return expr;
            }

            if(std::regex_match(token, CPEG::stringTokenRegex)) {
                expr.type = CPEGExpression_String;
                expr.value = token.substr(1, token.size() - 2);
                expr.props = props;

                return expr;
            }

            if(token == ".") {
                expr.type = CPEGExpression_WildCard;
                expr.value = token;
                expr.props = props;

                return expr;
            }

            throw CPEGException(CPEGException_InvalidCPEGSyntax);
        }

        /*
         * excep: CPEGException [ChoiceHasNoExpression] / CPEGTokensIndex::CPEGTokensIndex(std::vector<std::string>*, unsigned int, unsigned int) / getCPEGExpressionSequenceGroup(CPEGTokensIndex, CPEGExpressionSequenceGroup&)
         */
        static std::vector<CPEGExpressionChoice> getCPEGExpressionChoices(CPEGTokensIndex ruleTokensIndex) {
            if(CPEG::isCPEGTokensEmpty(ruleTokensIndex))
                throw CPEGException(CPEGException_ChoiceHasNoExpression);

            std::vector<CPEGExpressionChoice> choices;

            std::vector<std::string> *tokens = ruleTokensIndex.tokens();

            unsigned int begin = ruleTokensIndex.begin();
            unsigned int end = ruleTokensIndex.end();

            unsigned int seqGroupBegin = begin;

            try {
                for(int i = begin; i <= end; i++) {
                    if(i == end || tokens->at(i) == ">") {
                        int seqGroupEnd = i;

                        CPEGTokensIndex choiceTokensIndex(tokens, seqGroupBegin, seqGroupEnd - seqGroupBegin);

                        CPEGExpressionSequenceGroup group;

                        if(!CPEG::getCPEGExpressionSequenceGroup(choiceTokensIndex, group))
                            throw CPEGException(CPEGException_ChoiceHasNoExpression);

                        CPEGExpressionChoice newChoice;
                        newChoice.exprSeqGroup = group;

                        choices.push_back(newChoice);

                        seqGroupBegin = seqGroupEnd + 1;
                    }
                }
            } catch(CPEGException excep) {
                throw excep;
            }

            return choices;
        }

        /*
         * excep: CPEGException [InvalidSequenceGroupParen, SequenceGroupHasNoExpression]
         */
        static bool getCPEGExpressionSequenceGroup(CPEGTokensIndex choiceTokensIndex, CPEGExpressionSequenceGroup &group) {
            if(CPEG::isCPEGTokensEmpty(choiceTokensIndex))
                return false;

            CPEGExpressionSequenceGroup newGroup;

            std::vector<std::string> *tokens = choiceTokensIndex.tokens();

            unsigned int begin = choiceTokensIndex.begin();
            unsigned int end = choiceTokensIndex.end();

            unsigned int seqBegin = begin;
            int seqBeginInParen = begin;

            bool inParen = false;

            std::vector<CPEGExpression> tmpExprs;

            int i = begin;

            for(; i < end; i++) {
                if(tokens->at(i) == " ") {
                    CPEGTokensIndex groupTokensIndex(tokens, seqBegin, i - seqBegin);
                    CPEGExpression expr;

                    if(CPEG::getCPEGExpression(groupTokensIndex, expr))
                        tmpExprs.push_back(expr);

                    seqBegin = i + 1;

                    continue;
                }

                if(tokens->at(i) == "(") {
                    if(inParen)
                        throw CPEGException(CPEGException_InvalidSequenceGroupParen);

                    inParen = true;

                    if(tmpExprs.size() > 0) {
                        CPEGExpressionSequence newSeq;
                        newSeq.exprs = tmpExprs;

                        newGroup.exprSeqs.push_back(newSeq);

                        tmpExprs.clear();
                    }

                    seqBegin = i + 1;
                    seqBeginInParen = i;

                    // note: 開き丸括弧が最初のトークンでない場合のみスペースまたは先読み記号を含める
                    if(i - 1 >= 0)
                        seqBeginInParen = i - 1;

                    continue;
                }

                if(tokens->at(i) == ")") {
                    if(!inParen)
                        throw CPEGException(CPEGException_InvalidSequenceGroupParen);

                    inParen = false;

                    CPEGTokensIndex groupTokensIndex(tokens, seqBegin, i - seqBegin);
                    CPEGExpression expr;

                    if(CPEG::getCPEGExpression(groupTokensIndex, expr))
                        tmpExprs.push_back(expr);

                    if(tmpExprs.size() == 0)
                        throw CPEGException(CPEGException_SequenceGroupHasNoExpression);

                    int seqEndInParen = i;

                    if(i < tokens->size())
                        seqEndInParen = i + 1;

                    std::string firstStr = tokens->at(seqBeginInParen);
                    std::string lastStr = tokens->at(seqEndInParen);

                    CPEGExpressionProperties seqProps = CPEG::getCPEGExpressionProperties(firstStr, lastStr);

                    CPEGExpressionSequence newSeq;
                    newSeq.exprs = tmpExprs;
                    newSeq.props = seqProps;

                    newGroup.exprSeqs.push_back(newSeq);

                    tmpExprs.clear();

                    seqBegin = i + 2;
                    i += 1;

                    continue;
                }
            }

            if(begin != end) {
                if(inParen)
                    throw CPEGException(CPEGException_InvalidSequenceGroupParen);

                CPEGTokensIndex groupTokensIndex(tokens, seqBegin, i - seqBegin);
                CPEGExpression expr;

                if(CPEG::getCPEGExpression(groupTokensIndex, expr))
                    tmpExprs.push_back(expr);

                if(tmpExprs.size() > 0) {
                    CPEGExpressionSequence newSeq;
                    newSeq.exprs = tmpExprs;

                    newGroup.exprSeqs.push_back(newSeq);
                }
            }

            group = newGroup;

            return true;
        }

        static CPEGExpressionProperties getCPEGExpressionProperties(std::string firstStr, std::string lastStr) noexcept {
            char firstChar = '\0';
            char lastChar = '\0';

            if(firstStr.size() != 0)
                firstChar = firstStr.at(0);

            if(lastStr.size() != 0)
                lastChar = lastStr.at(0);

            return CPEG::getCPEGExpressionProperties(firstChar, lastChar);
        }

        static CPEGExpressionProperties getCPEGExpressionProperties(char firstChar, char lastChar) noexcept {
            CPEGExpressionProperties props;

            int symbolCount = 0;

            CPEGExpressionLoopType loopType = CPEGExpressionProperties::toLoopType(lastChar);

            if(loopType != CPEGExpressionLoop_Default) {
                props.loopType = loopType;
                symbolCount++;
            }

            CPEGExpressionLookbehindType lookbehindType = CPEGExpressionProperties::toLookbehindType(firstChar);

            if(lookbehindType != CPEGExpressionLookbehind_Default) {
                props.lookbehindType = lookbehindType;
                symbolCount++;
            }

            if(symbolCount == 0)
                return CPEGExpressionProperties();

            return props;
        }

        static CPEGExpressionProperties getCPEGExpressionProperties(std::string &token) noexcept {
            if(token.size() == 0)
                return CPEGExpressionProperties();

            char firstChar = token.at(0);
            char lastChar = token.at(token.size() - 1);

            CPEGExpressionProperties props = CPEG::getCPEGExpressionProperties(firstChar, lastChar);

            if(props.lookbehindType != CPEGExpressionLookbehind_Default)
                token = token.substr(1);

            if(token.size() == 0)
                return props;

            if(props.loopType != CPEGExpressionLoop_Default)
                token = token.substr(0, token.size() - 1);

            return props;
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
                newRule.exprChoices = CPEG::getCPEGExpressionChoices(CPEGTokensIndex(&tokens, ruleDefStartIndex));
            }

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
                    if(line.at(i) == '#') {
                        tokens.push_back({ line.at(i) });

                        tmpToken = "";

                        break;
                    }

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

                    if(line.at(i) == '[') {
                        tmpToken.push_back(line.at(i));

                        for(i++; i < line.size(); i++) {
                            tmpToken.push_back(line.at(i));

                            if(line.at(i) == ']') {
                                tokens.push_back(tmpToken);
                                break;
                            }
                        }

                        tmpToken = "";

                        continue;
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
