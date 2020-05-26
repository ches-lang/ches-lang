#pragma once

typedef unsigned char           Byte;
typedef std::vector<Byte>       ByteSeq;
typedef std::vector<ByteSeq>    TokenSeq;
typedef std::vector<TokenSeq>   LineSeq;



/* ! ? ~ + - * / % ^ = | & . , : ; ( ) [ ] < > { } */
enum TokenType : Byte {
    TK_Unknown,
    TK_EndOfFile,
    TK_Indent,
    TK_NewLine,
    TK_Comment,
    TK_Keyword,
    TK_Identifier,
    TK_Number,
    TK_Char,
    TK_String,
    TK_Exclamation,
    TK_Question,
    TK_Tilde,
    TK_Plus,
    TK_Hyphen,
    TK_Asterisk,
    TK_Slash,
    TK_Percent,
    TK_Caret,
    TK_Equal,
    TK_Pipe,
    TK_Ampersand,
    TK_Period,
    TK_Comma,
    TK_Colon,
    TK_Semicolon,
    TK_LeftParen,
    TK_RightParen,
    TK_LeftBracket,
    TK_RightBracket,
    TK_LeftAngleBracket,
    TK_RightAngleBracket,
    TK_LeftBrace,
    TK_RightBrace,
};



enum NodeType : Byte {
    ND_Unknown,
    ND_Root,
    ND_Token,
    ND_DefVar,
    ND_InitVar,
    ND_DefFunc,
    ND_CallFunc,
    ND_Args,
    ND_If,
    ND_Else,
    ND_ElseIf,
    ND_Loop,
    ND_Loop_Cond,
    ND_Loop_Init,
    ND_Loop_Change,
    ND_Count,
    ND_Logic,
    ND_Compare,
    ND_Expression,
    ND_Operator,
    ND_Equal,
    ND_Less,
    ND_Greater,
    ND_LessEqual,
    ND_GreaterEqual,
};



enum InstType : Byte {
    IT_Unknown,
    IT_LineDiv,
    IT_TokenDiv,
    IT_Label,
    IT_LSPush,
    IT_LLPush,
    IT_Logic_And,
    IT_Logic_Or,
    IT_Calc_Add,
    IT_Calc_Sub,
    IT_Calc_Multi,
    IT_Calc_Div,
    IT_Jump,
    IT_IFJump,
};



struct Token {

    int index = 0;
    Byte type = TK_Unknown;
    std::string string = "";

    Token();

    Token(Byte type);

    Token(std::string string);

    Token(Byte type, std::string string);

    Token(Byte type, std::string string, int index);

    bool compare(Token token);

    Token getCloseParen();

    Token getOpenParen();

    std::pair<int, int> getPosition(std::string source);

    static std::pair<int, int> getPosition(std::string source, int index) {
        int line = 0;
        int pos = 0;

        for(int i = 0; i < index; i++) {
            if(source[i] == '\n') {
                pos = 0;
                line++;
            } else {
                pos++;
            }
        }

        return { line, pos };
    }

    static std::string getPositionText(std::string filePath, std::string source, int index) {
        std::pair pos = Token::getPosition(source, index);
        return filePath + ":" + std::to_string(pos.first + 1) + ":" + std::to_string(pos.second + 1);
    }

    std::string getPositionText(std::string filePath, std::string source);

    bool isParen();

    bool isCloseParen();

    bool isOpenParen();

    bool match(ByteSeq matches);

    bool match(std::string regexp);
};



std::unordered_map<Byte, std::string> nodeTypeMap {
    { ND_Unknown, "Unknown" },
    { ND_Root, "Root" },
    { ND_Token, "Token" },
    { ND_DefVar, "DefVar" },
    { ND_InitVar, "InitVar" },
    { ND_DefFunc, "DefFunc" },
    { ND_CallFunc, "CallFunc" },
    { ND_Args, "Args" },
    { ND_If, "If" },
    { ND_Else, "Else" },
    { ND_ElseIf, "ElseIf" },
    { ND_Loop, "Loop" },
    { ND_Loop_Cond, "Loop_Cond" },
    { ND_Loop_Init, "Loop_Init" },
    { ND_Loop_Change, "Loop_Change" },
    { ND_Count, "Count" },
    { ND_Logic, "Logic" },
    { ND_Compare, "Comp" },
    { ND_Expression, "Exp" },
    { ND_Operator, "Ope" },
    { ND_Equal, "Equal" },
    { ND_Less, "Less" },
    { ND_Greater, "Greater" },
    { ND_LessEqual, "LessEqual" },
    { ND_GreaterEqual, "GreaterEqual" },
};



struct Node {

    Byte type = ND_Unknown;
    std::vector<Node> children;
    std::vector<Token> tokens;
    std::string prefix = "||";

    Node();

    Node(Byte type);

    Node(Byte type, std::vector<Node> children);

    Node(Byte type, std::vector<Token> tokens);

    Node(Byte type, std::vector<Node> children, std::vector<Token> tokens);

    void addChild(Node node);

    void addToken(Token token);

    Node childAt(int index);

    Token tokenAt(int index);

    void print();

    void print(std::string level);

    std::string typeToString();
};
