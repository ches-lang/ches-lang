/* 
 * 
 * Chestnut 0.0.0
 * 
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 * 
 * Copyright © 2020 Garnet3106 All rights reserved. 
 * 
 */


#pragma once


namespace ches {
    enum SyntaxNotationErrorType {
        SyntaxNotation_Unknown,
        SyntaxNotation_InvalidSyntax,
        SyntaxNotation_UnknownLabelName
    };


    class SyntaxNotationError : std::exception {
    public:
        SyntaxNotationErrorType type = SyntaxNotation_Unknown;

        SyntaxNotationError();

        SyntaxNotationError(SyntaxNotationErrorType type);
    };


    enum SyntaxItemType {
        SyntaxItem_Unknown,
        SyntaxItem_ID,
        SyntaxItem_RegExp,
        SyntaxItem_String
    };


    typedef std::pair<SyntaxItemType, std::string>  syntax_token;
    typedef std::vector<std::vector<std::string>>   syntax_token_lines;
    typedef std::vector<syntax_token>               syntax_item;
    typedef std::vector<syntax_item>                syntax_item_seq;


    struct SyntaxProp {
    public:
        std::string name = "";
        syntax_item_seq items = {};

        SyntaxProp();

        SyntaxProp(std::string name);

        SyntaxProp(std::string name, syntax_item_seq items);
    };


    typedef std::unordered_map<std::string, std::vector<SyntaxProp>>    syntax_prop_map;


    struct Syntax {
    public:
        syntax_prop_map syntaxProps = {
            { "tokens", {} },
            { "expressions", {} },
            { "statements", {} }
        };

        Syntax();

        void load(std::string filePath);

    private:
        syntax_token getSyntaxToken(std::string token) {
            SyntaxItemType type;

            std::regex strPattern("\".*\"");
            std::regex regExpPattern("/.*/");
            std::regex idPattern("[a-zA-Z0-9_]+");

            if(token.size() == 0) {
                type = SyntaxItem_Unknown;
            } else if(std::regex_match(token, strPattern)) {
                type = SyntaxItem_String;
            } else if(std::regex_match(token, regExpPattern)) {
                type = SyntaxItem_RegExp;
            } else if(std::regex_match(token, idPattern)) {
                type = SyntaxItem_ID;
            } else {
                type = SyntaxItem_Unknown;
            }

            return std::make_pair(type, token);
        }

        void loadProps(syntax_token_lines lines);

        static syntax_token_lines toTokenLines(std::vector<std::string> &fileContent) {
            syntax_token_lines tokenLines;

            std::regex symbols("[ #=]");
            std::regex encloserSymbols("[\"/]");

            for(std::string line : fileContent) {
                if(line.size() >= 1 && line.at(0) == ';')
                    continue;

                std::vector<std::string> tokenSeq;
                std::string tmpToken = "";

                std::string encloser = "";

                for(int i = 0; i < line.size(); i++) {
                    std::string ch = line.substr(i, 1);

                    if(encloser == "" && std::regex_match(ch, symbols)) {
                        if(tmpToken.size() != 0) {
                            tokenSeq.push_back(tmpToken);
                            tmpToken = "";
                        }

                        if(ch != " ")
                            tokenSeq.push_back(ch);
                    } else {
                        if(std::regex_match(ch, encloserSymbols) && (encloser == "" || encloser == ch)) {
                            if(encloser == "") {
                                encloser = ch;
                            } else {
                                encloser = "";
                            }

                            tmpToken += ch;
                        } else if(ch == "\\" && i + 1 < line.size()) {
                            tmpToken += line.at(i + 1);
                            i++;
                        } else {
                            tmpToken += ch;
                        }
                    }

                    if(i + 1 == line.size() && tmpToken.size() != 0)
                        tokenSeq.push_back(tmpToken);
                }

                tokenLines.push_back(tokenSeq);
            }

            return tokenLines;
        }
    };
};
