#pragma once

#include "syntax.hpp"



Token::Token() {}

Token::Token(unsigned char type) {
    this->type = type;
}

Token::Token(std::string string) {
    this->string = string;
}

Token::Token(unsigned char type, std::string string) {
    this->type = type;
    this->string = string;
}

Token::Token(unsigned char type, std::string string, int index) {
    this->type = type;
    this->string = string;
    this->index = index;
}

// Index won't be compared.
bool Token::compare(Token token) {
    return (this->type == token.type && this->string == token.string);
}

Token Token::getCloseParen() {
    switch (this->type) {
        case LPAREN:
        return Token(RPAREN, ")", this->index);

        case LBRACK:
        return Token(RBRACK, "]", this->index);

        case LBRACE:
        return Token(RBRACE, "}", this->index);

        case LANGBRACK:
        return Token(RANGBRACK, ">", this->index);
    }

    return Token();
}

Token Token::getOpenParen() {
    switch (this->type) {
        case RPAREN:
        return Token(LPAREN, "(", this->index);

        case RBRACK:
        return Token(LBRACK, "[", this->index);

        case RBRACE:
        return Token(LBRACE, "{", this->index);

        case RANGBRACK:
        return Token(LANGBRACK, "<", this->index);
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

bool Token::match(std::vector<unsigned char> matches) {
    bool res = false;

    for(int i = 0; i < matches.size(); i++) {
        if(matches[i] == this->type) {
            res = true;
        }
    }

    return res;
}

bool Token::match(std::string regexp) {
    return std::regex_match(this->string, std::regex(regexp));
}



Node::Node() {}

Node::Node(unsigned char type) {
    this->type = type;
}

Node::Node(unsigned char type, std::vector<Node> children) {
    this->type = type;
    this->children = children;
}

Node::Node(unsigned char type, std::vector<Token> tokens) {
    this->type = type;
    this->tokens = tokens;
}

Node::Node(unsigned char type, std::vector<Node> children, std::vector<Token> tokens) {
    this->type = type;
    this->children = children;
    this->tokens = tokens;
}

void Node::addChild(Node node) {
    this->children.push_back(node);
}

// トークン配列, 呼び出し元のパーサ(ポインタ)
//Node::addChild(std::vector<Token> tokens, Parser* parser) {
    //if(tokens.size() >= 1) this->addChild(parser->getNode(tokens));
    //else this->addChild(Node(N_TOKEN, tokens));
//}

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
    std::cout << "\033[31m" << level << "\033[m" << std::hex << (int)this->type << std::endl;

    for(Token token : this->tokens)
        std::cout << "\033[31m" << level << this->prefix << "\033[m" << ((token.string == "|") ? "[pipe]" : token.string) << std::endl;

    for(Node node : this->children)
        node.print(level + this->prefix);
}
