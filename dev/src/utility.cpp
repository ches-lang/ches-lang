#pragma once

#include "utility.hpp"



Token::Token() {}

Token::Token(Byte type) {
    this->type = type;
}

Token::Token(std::string string) {
    this->string = string;
}

Token::Token(Byte type, std::string string) {
    this->type = type;
    this->string = string;
}

Token::Token(Byte type, std::string string, int index) {
    this->type = type;
    this->string = string;
    this->index = index;
}

// インデックスは比較されません
bool Token::compare(Token token) {
    return this->type == token.type && this->string == token.string;
}

Token Token::getCloseParen() {
    switch(this->type) {
        case TK_LeftParen:
        return Token(TK_RightParen, ")", this->index);

        case TK_LeftBracket:
        return Token(TK_RightBracket, "]", this->index);

        case TK_LeftBrace:
        return Token(TK_RightBrace, "}", this->index);

        case TK_LeftAngleBracket:
        return Token(TK_RightAngleBracket, ">", this->index);
    }

    return Token();
}

Token Token::getOpenParen() {
    switch (this->type) {
        case TK_RightParen:
        return Token(TK_LeftParen, "(", this->index);

        case TK_RightBracket:
        return Token(TK_LeftBracket, "[", this->index);

        case TK_RightBrace:
        return Token(TK_LeftBrace, "{", this->index);

        case TK_RightAngleBracket:
        return Token(TK_LeftAngleBracket, "<", this->index);
    }

    return Token();
}

std::pair<int, int> Token::getPosition(std::string source) {
    int line = 0;
    int pos = 0;

    for(int i = 0; i < this->index; i++) {
        if(source[i] == '\n') {
            pos = 0;
            line++;
        } else {
            pos++;
        }
    }

    return { line, pos };
}

std::string Token::getPositionText(std::string filePath, std::string source) {
    std::pair pos = this->getPosition(source);
    return filePath + ":" + std::to_string(pos.first + 1) + ":" + std::to_string(pos.second + 1);
}

bool Token::isParen() {
    return this->match(ByteSeq { TK_LeftParen, TK_LeftBracket, TK_LeftBrace, TK_RightParen, TK_RightBracket, TK_RightBrace });
}

bool Token::isCloseParen() {
    return this->match(ByteSeq { TK_RightParen, TK_RightBracket, TK_RightBrace });
}

bool Token::isOpenParen() {
    return this->match(ByteSeq { TK_LeftParen, TK_LeftBracket, TK_LeftBrace });
}

bool Token::match(ByteSeq matches) {
    bool res = false;

    for(int i = 0; i < matches.size(); i++)
        if(matches[i] == this->type)
            res = true;

    return res;
}

bool Token::match(std::string regexp) {
    return std::regex_match(this->string, std::regex(regexp));
}



Node::Node() {}

Node::Node(Byte type) {
    this->type = type;
}

Node::Node(Byte type, std::vector<Node> children) {
    this->type = type;
    this->children = children;
}

Node::Node(Byte type, std::vector<Token> tokens) {
    this->type = type;
    this->tokens = tokens;
}

Node::Node(Byte type, std::vector<Node> children, std::vector<Token> tokens) {
    this->type = type;
    this->children = children;
    this->tokens = tokens;
}

void Node::addChild(Node node) {
    this->children.push_back(node);
}

void Node::addToken(Token token) {
    this->tokens.push_back(token);
}

Node Node::childAt(int index) {
    return this->children[(index >= 0 ? index : this->children.size() + index)];
}

Token Node::tokenAt(int index) {
    return this->tokens[(index >= 0 ? index : this->tokens.size() + index)];
}

void Node::print() {
    this->print(this->prefix);
}

void Node::print(std::string level) {
    std::cout << "\033[44m" << level << "\033[m" << " " << "\033[94m" << this->typeToString() << "\033[m" << std::endl;

    for(Token token : this->tokens)
        std::cout << "\033[44m" << level << this->prefix << "\033[m" << " " << token.string << std::endl;

    for(Node node : this->children)
        node.print(level + this->prefix);
}

std::string Node::typeToString() {
    if(nodeTypeMap.count(this->type) == 1) {
        return nodeTypeMap[this->type];
    } else {
        return "???";
    }
}



ByteSeq::ByteSeq() {}

ByteSeq::ByteSeq(Byte value) {
    this->push_back(value);
}

ByteSeq::ByteSeq(Byte value, int len) {
    for(int i = 0; i < len; i++)
        this->push_back(value);
}

ByteSeq::ByteSeq(std::initializer_list<Byte> value) {
    for(Byte val : value)
        this->push_back(val);
}

ByteSeq::ByteSeq(int source) {
    std::stringstream ss;
    ss << std::hex << source;
    std::string hex = ss.str();
    ByteSeq hexBytes;
    bool isEvenDigNum = hex.length() % 2 == 0;

    if(!isEvenDigNum)
        hexBytes.push_back((Byte)std::stoi("0" + std::string { hex[0] }, nullptr, 16));

    for(int i = isEvenDigNum ? 0 : 1; i < hex.length(); i += 2)
        hexBytes.push_back((Byte)std::stoi(std::string { hex[i] } + std::string { hex[i + 1] }, nullptr, 16));

    this->push_back((Byte)source);
}

ByteSeq::ByteSeq(std::string source) {
    for(Byte src : source)
        this->push_back(src);
}

ByteSeq::ByteSeq(Node tree, std::string filePath, std::string source) {
    // ヘッダ部分

    HeaderInfo header;
    header.magicNum = MAGIC_NUMBER;
    this->push_back(header.toByteSeq());

    // ボディ部分

    InstList instList = InstList(tree, filePath, source);
    ByteSeq byteSeq = instList.toByteSeq();
    this->push_back(byteSeq);

    std::cout << std::endl;
}

ByteSeq ByteSeq::copy(int begin, int end) {
    ByteSeq result;

    auto beginItr = begin >= 0 ? this->begin() + begin : this->begin() + this->size() + begin + 1;
    auto endItr = end >= 0 ? this->end() - this->size() + end + 1 : this->end() + end + 1;

    std::copy(beginItr, endItr, std::back_inserter(result));

    return result;
}

ByteSeq ByteSeq::escape() {
    ByteSeq result;

    for(int i = 0; i < this->size(); i++) {
        result.push_back(this->at(i));

        if(this->at(i) == IT_LineDiv || this->at(i) == IT_TokenDiv) {
            result.push_back(this->at(i));
            i++;
        }
    }

    return result;
}

std::string ByteSeq::toHexString(std::string sep) {
    std::string result;

    for(int src : *this) {
        std::stringstream ss;
        ss << std::hex << src;
        result += (src < 16 ? "0" : "") + ss.str() + sep;
    }

    if(sep.length() && sep.length() >= result.length())
        this->pop_back(sep.length());

    return result;
}

int ByteSeq::toInt() {
    return *(unsigned int*)&(*this);///
}

LineSeq ByteSeq::toLineSeq() {
    LineSeq lines = {{}};

    try {
        for(int i = 0; i < this->size(); i++) {
            if(this->at(i) == IT_LineDiv || this->at(i) == IT_TokenDiv) {
                if(i + 1 < this->size() && this->at(i + 1) == this->at(i)) {
                    lines.back().push_back(this->at(i));
                    i++;
                } else {
                    lines.push_back();
                }
            } else {
                lines.back().push_back(this->at(i));
            }
        }
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
        return lines;
    }

    return lines;
}

std::string ByteSeq::toString() {
    std::string res;

    for(Byte srcChar : *this)
        res += (char)srcChar;

    return res;
}



LineSeq::LineSeq() {}

LineSeq::LineSeq(ByteSeq value) {
    this->push_back(value);
}

LineSeq::LineSeq(std::initializer_list<ByteSeq> value) {
    for(ByteSeq val : value)
        this->push_back(val);
}

LineSeq::LineSeq(std::vector<ByteSeq> value) {
    this->push_back(value);
}



Instruction::Instruction(ByteSeq bytes) {
    this->init(bytes);
}

Instruction::Instruction(int opcode) {
    this->opcode = opcode;
    this->operand = {};
    this->setBytecode();
}

Instruction::Instruction(int opcode, std::unordered_map<std::string, ByteSeq> operand) {
    this->opcode = opcode;
    this->operand = operand;
    this->setBytecode();
}

std::string Instruction::toText() {
    std::string text = "";

    switch(this->opcode) {
        case IT_Unknown:
        return "; Unknown";

        case IT_Label:
        return "label\t" + (this->operand["id"]).toHexString() + "\t" + ByteSeq(this->operand["name"]).toString();

        case IT_Jump:
        return "jump\t" + ByteSeq(this->operand["index"]).toHexString();

        case IT_IFJump:
        return "ifjump\t" + ByteSeq(this->operand["index"]).toHexString();

        default:
        return "; Unknown_";
    }
}



void Instruction::setBytecode() {
    try {
        switch(this->opcode) {
            case IT_Unknown: {
                this->bytecode.push_back((Byte)IT_Unknown);
            } break;

            case IT_Label: {
                this->bytecode.push_back((Byte)IT_Label);
                this->bytecode.push_back(this->operand["id"]);
                this->bytecode.push_back(this->operand["name"].escape());
            } break;

            case IT_Jump: {
                this->bytecode.push_back((Byte)IT_Jump);
                this->bytecode.push_back(this->operand["index"].escape());
            } break;

            case IT_IFJump: {
                this->bytecode.push_back((Byte)IT_IFJump);
                this->bytecode.push_back(this->operand["index"].escape());
            } break;

            default: {
                this->bytecode.push_back((Byte)IT_Unknown);
            } break;
        }
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}

void Instruction::init(ByteSeq bytes) {
    try {
        if(bytes.size() == 0)
            bytes = { IT_Unknown };

        this->bytecode = bytes;
        this->opcode = bytes[0];

        switch(this->opcode) {
            case IT_Label: {
                this->operand["id"] = this->bytecode.copy(1, 16);
                this->operand["name"] = this->bytecode.copy(17 , -1);
            } break;

            case IT_Jump: {
                this->operand["index"] = this->bytecode.copy(1, -1);
            } break;

            case IT_IFJump: {
                this->operand["index"] = this->bytecode.copy(1, -1);
            } break;
        }
    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
    }
}



InstList::InstList() {
    FuncList tmp;
    this->labelList = &tmp;
}

InstList::InstList(Instruction value) {
    FuncList tmp;
    this->labelList = &tmp;
    this->push_back(value);
}

InstList::InstList(std::initializer_list<Instruction> value) {
    FuncList tmp;
    this->labelList = &tmp;

    for(Instruction val : value)
        this->push_back(val);
}

InstList::InstList(std::vector<Instruction> value) {
    FuncList tmp;
    this->labelList = &tmp;
    this->push_back(value);
}

InstList::InstList(Node tree, std::string filePath, std::string source) {
    FuncList tmp;
    this->labelList = &tmp;
    this->filePath = filePath;
    this->source = source;

    for(Node node : tree.children)
        if(node.type == ND_DefFunc)
            this->labelList->push_back(Function(ByteSeq::generateUUID(), ByteSeq(node.tokenAt(0).string)));

    for(int i = 0; i < tree.children.size(); i++)
        this->push_back(this->toInstList(tree, i));
}

ByteSeq InstList::toByteSeq() {
    ByteSeq byteSeq;

    for(Instruction inst : *this) {
        byteSeq.push_back(inst.bytecode);
        byteSeq.push_back((Byte)IT_LineDiv);
    }

    if(this->size() > 0)
        byteSeq.pop_back();

    return byteSeq;
}

InstList InstList::toInstList(Node parentNode, int &index) {
    Node node = parentNode.childAt(index);
    std::cout << std::endl << (int)index << " > " << parentNode.children.size() << " | " << (int)node.type << std::endl;
    InstList instList;

    try {

        switch(node.type) {
            case ND_Unknown: {std::cout<<"unknown"<<std::endl;
                instList.push_back(Instruction(IT_Unknown));
            } break;

            case ND_Root: {std::cout<<"root"<<std::endl;
                for(int i = 0; i < node.children.size(); i++) {
                    InstList resLines = this->toInstList(node, i);
                    instList.push_back(resLines);
                }
            } break;

            case ND_DefVar: {std::cout<<"defvar"<<std::endl;
                instList.push_back(Instruction(IT_LSPush, {}));//{}
                int i = 1;
                this->toInstList(node, i);
                this->localStackLen++;
            } break;

            case ND_InitVar: {std::cout<<"initvar"<<std::endl;
                Token token = node.tokenAt(2);
                ByteSeq value;

                if(token.type == TK_Number)
                    value = ByteSeq(std::stoi(token.string));
                else if(token.type == TK_String)
                    value = ByteSeq(token.string);

                instList.push_back(Instruction(IT_LSPush, { { "index", value } }));
                this->localStackLen++;
            } break;

            case ND_DefFunc: {std::cout<<"deffunc"<<std::endl;
                ByteSeq funcName = ByteSeq(node.tokenAt(0).string);
                ByteSeq funcID = ByteSeq(this->labelList->findByName(funcName).id);
                instList.push_back(Instruction(IT_Label, { { "id", funcID }, { "name", funcName } }));
                this->localListLen += node.childAt(0).children.size();

                int i = 1;
                InstList insts = this->toInstList(node, i);
                instList.push_back(insts);
            } break;

            case ND_CallFunc: {std::cout<<"callfunc"<<std::endl;
                Token funcNameToken = node.tokenAt(0);
                std::string funcName = funcNameToken.string;
                ByteSeq funcID = this->labelList->findByName(ByteSeq(funcName)).id;

                if(funcID.size() == 0)
                    Console::log(LogType_Error, 1822, { { "At", funcNameToken.getPositionText(this->filePath, this->source ) }, { "Id", funcName } }, false);

                instList.push_back(Instruction(IT_Jump, { { "id", funcID } }));
            } break;

            case ND_If: {std::cout<<"if"<<std::endl;
                // 次のノードに else / elseif がくれば

                if(index + 1 < parentNode.children.size()) {
                    int nextNodeType = parentNode.childAt(index + 1).type;

                    if(nextNodeType == ND_Else || nextNodeType == ND_ElseIf) {
                        //
                        std::cout<<"before else or elseif"<<std::endl;
                    } else {
                        std::cout << "not before else or elseif"<<nextNodeType<<std::endl;
                    }
                }

                // 条件式をチェック

                int i = 1;
                InstList procLines = this->toInstList(node, i);

                // procLineの行数をもとにIFJUMP命令を追加
                ByteSeq lineIndex(IT_VarPref);
                lineIndex.push_back(ByteSeq((int)procLines.size()));
                instList.push_back(Instruction(IT_IFJump, { { "index", lineIndex } }));

                instList.push_back(procLines);
            } break;

            case ND_Else: {std::cout<<"else"<<std::endl;
                int i = 0;
                InstList insts = this->toInstList(node, i);
                instList.push_back(insts);
            } break;
        }

        return instList;

    } catch(std::out_of_range ignored) {
        std::cout << "EXCEPTION" << std::endl;
        return instList;
    }
}



Function::Function() {}

Function::Function(ByteSeq id, ByteSeq name) {
    this->id = id;
    this->name = name;
}

Function::Function(ByteSeq id, ByteSeq name, int begin, int end) {
    this->id = id;
    this->name = name;
    this->begin = begin;
    this->end = end;
}



FuncList::FuncList() {}

FuncList::FuncList(Function value) {
    this->push_back(value);
}

FuncList::FuncList(std::initializer_list<Function> value) {
    for(Function val : value)
        this->push_back(val);
}

FuncList::FuncList(std::vector<Function> value) {
    this->push_back(value);
}

Function FuncList::findById(ByteSeq id) {
    for(Function func : *this)
        if(func.id == id)
            return func;

    return Function();
}

Function FuncList::findByName(ByteSeq name) {
    for(Function func : *this)
        if(func.name == name)
            return func;

    return Function();
}



Stack::Stack() {}



StackList::StackList() {}



HeaderInfo::HeaderInfo() {}

HeaderInfo::HeaderInfo(ByteSeq bytes) {
    if(bytes.size() != HEADER_LEN)
        return;

    // 各データのバイト数変更に対応するためのインデックス管理
    int index = 0;

    int magicNumSize = MAGIC_NUMBER.size();
    this->magicNum = bytes.copy(index, index + magicNumSize - 1);
    index += magicNumSize;
}

ByteSeq HeaderInfo::toByteSeq() {
    ByteSeq result;

    result.push_back(MAGIC_NUMBER);
    result.push_back(ByteSeq(0, (int)(HEADER_LEN - result.size())));

    return result;
}
