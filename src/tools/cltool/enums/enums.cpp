#pragma once


namespace ches {
    typedef unsigned char       Byte;
    typedef std::vector<Byte>   ByteVec;

    #define HEADER_LEN      (128)
    #define MAGIC_NUM       (ByteVec { 0x4F, 0x52, 0x49, 0x4E, 0x43, 0x48, 0x41, 0x4E })
    #define MAGIC_NUM_SEQ   (ByteSeq { 0x4F, 0x52, 0x49, 0x4E, 0x43, 0x48, 0x41, 0x4E })


    /* ! ? ~ + - * / % ^ = | & . , : ; ( ) [ ] < > { } */
    enum TokenType : Byte {
        TK_Unknown,
        TK_EndOfFile,
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
        TK_RightBrace
    };


    enum NodeType : Byte {
        ND_Unknown,
        ND_Root,
        ND_Value,
        ND_Object,
        ND_DefVar,
        ND_InitVar,
        ND_DefFunc,
        ND_CallFunc,
        ND_Args,
        ND_If,
        ND_Else,
        ND_ElseIf,
        ND_Loop,
        ND_Count,
        ND_Logic,
        ND_Compare,
        ND_Expression,
        ND_Operator,
        ND_Equal,
        ND_Less,
        ND_Greater,
        ND_LessEqual,
        ND_GreaterEqual
    };


    enum InstType : Byte {
        IT_Unknown,
        IT_InstDiv,
        IT_OpeDiv,
        IT_Add,
        IT_And,
        IT_ArgPref,
        IT_Block,
        IT_Div,
        IT_Equal,
        IT_Greater,
        IT_Join,
        IT_Jump,
        IT_JumpIf,
        IT_JumpIfNot,
        IT_Load,
        IT_Mul,
        IT_Or,
        IT_Pop,
        IT_Push,
        IT_Return,
        IT_Sub,
        IT_SysCall
    };


    enum ValueType : Byte {
        VT_Unknown,
        VT_Boolean,
        VT_Byte,
        VT_UByte,
        VT_Short,
        VT_UShort,
        VT_Int,
        VT_UInt,
        VT_Long,
        VT_ULong,
        VT_Char,
        VT_String,
        VT_Decimal
    };


    enum DataSizeType : Byte {
        DST_2,
        DST_8,
        DST_16,
        DST_32,
        DST_64
    };


    // tokenTypeMapを追加する


    std::unordered_map<Byte, std::string> nodeTypeMap {
        { ND_Unknown, "Unknown" },
        { ND_Root, "Root" },
        { ND_Value, "Value" },
        { ND_DefVar, "DefVar" },
        { ND_InitVar, "InitVar" },
        { ND_DefFunc, "DefFunc" },
        { ND_CallFunc, "CallFunc" },
        { ND_Args, "Args" },
        { ND_If, "If" },
        { ND_Else, "Else" },
        { ND_ElseIf, "ElseIf" },
        { ND_Loop, "Loop" },
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


    std::unordered_map<Byte, std::string> instTypeMap {
        { IT_Unknown, "Unknown" },
        { IT_InstDiv, "InstDiv" },
        { IT_OpeDiv, "OpeDiv" },
        { IT_Add, "Add" },
        { IT_And, "And" },
        { IT_ArgPref, "ArgPref" },
        { IT_Block, "Block" },
        { IT_Div, "Div" },
        { IT_Equal, "Equal" },
        { IT_Greater, "Greater" },
        { IT_Join, "Join" },
        { IT_Jump, "Jump" },
        { IT_JumpIf, "JumpIf" },
        { IT_JumpIfNot, "JumpIfNot" },
        { IT_Load, "Load" },
        { IT_Mul, "Mul" },
        { IT_Or, "Or" },
        { IT_Pop, "Pop" },
        { IT_Push, "Push" },
        { IT_Return, "Return" },
        { IT_Sub, "Sub" },
        { IT_SysCall, "SysCall" }
    };


    std::unordered_map<Byte, std::string> valueTypeMap {
        { VT_Unknown, "" },
        { VT_Boolean, "bol" },
        { VT_Byte, "byt" },
        { VT_UByte, "ubyt" },
        { VT_Short, "sht" },
        { VT_UShort, "usht" },
        { VT_Int, "int" },
        { VT_UInt, "uint" },
        { VT_Long, "lng" },
        { VT_ULong, "ulng" },
        { VT_Decimal, "dec" },
        { VT_Char, "chr" },
        { VT_String, "str" }
    };


    std::unordered_map<std::string, DataSizeType> dataSizeTypeMap {
        { "bol", DST_2 },
        { "byt", DST_8 },
        { "ubyt", DST_8 },
        { "sht", DST_16 },
        { "usht", DST_16 },
        { "int", DST_32 },
        { "uint", DST_32 },
        { "lng", DST_64 },
        { "ulng", DST_64 },
        { "dec", DST_32 },
        { "chr", DST_8 },
        { "str", DST_32 }
    };
}
