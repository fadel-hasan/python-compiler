/*
* @name python_ast_node.h
* @description Hierachy of AST
* @author Ismaeel_J_K 
* @example x=y+99
*                        |---------|
*                        | AstNode |
*                        |---------|
*                             |
*                             |
*                      |--------------|
*                      |    Assign    |
*                      |--------------|
*                          |        |
*                         |           |
*                        |              |
*                    |------------|   |--------------|
*                    |     x      |   |      Plus    |
*                    |------------|   |--------------|
*                                       |          |
*                                      |            |
*                                     |              |
*                           |-------------|     |-------------|
*                           |       y     |     |       99    |
*                           |-------------|     |-------------|
*                                       
*/

#ifndef AST_NODE_H
#define AST_NODE_H

#include <iostream>
#include <vector>
// #include <stdlib.h>


// Abstract base class for AST nodes
class AstNode {
public:
    std::string name = "undefined";   // String member variable with default value
    std::string label = "undefined";
    virtual void add(AstNode* node) = 0;
    virtual void print() const = 0;
    virtual ~AstNode() {}
    
};




// Composite node for representing function declare
class FunctionNode : public AstNode {
private:
    std::vector<AstNode*> next;

public:
    FunctionNode(const std::string& name) {
        this->name = name;
        this->label = "Declare Fun";
    }

    void add(AstNode* node) override {
        next.push_back(node);
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label <<" : "<<name<<"\"]" << std::endl;
        std::vector<AstNode*>::iterator it;
        // for (it = next.begin(); it != next.end(); ++it) {
        //     // example
        //     // Func -> args
        //     // Func -> body
        //     std::cout << "\t" << name << " -> " << (*it)->name << ";" << std::endl;
        //     (*it)->print();
        // }
        for (const auto& item : next) {
            std::cout << "\t" << name << " -> " << item->name << ";" << std::endl;
            item->print();
        }
    }

    ~FunctionNode() {
        for (const auto& arg : next) {
            delete arg;
        }
    }
};

// base node for representing identifier ,will create object  from lexer
class IdentifierNode : public AstNode {

public:
    std::string value = "undefined";
    IdentifierNode(std::string name, std::string label, std::string value) {
        this->name = name;
        this->label = label;
        this->value = value; 
    }
    void add(AstNode* /*node*/) override {
        std::cerr << "Cannot add a child to a leaf node." << std::endl;
    }

    void print() const override {
        std::cout << "\t" << name << " [shape=box,label=\"" << label << ": " << value << "\"]" << std::endl;

    }
};


class Arg : public AstNode {
private:
    std::vector<AstNode*> next;

public:
    Arg(const std::string& name) {
        this->name = name;
        this->label = "Argument";
    }

    void add(AstNode* node) override {
        next.push_back(node);
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << "\"]" << std::endl;
        for (const auto& arg : next) {
            arg->print();
        }
    }
};



class Args : public AstNode {
private:
    std::vector<AstNode*> next;

public:
    Args(const std::string& name) {
        this->name = name;
        this->label = "Arguments";
    }

    void add(AstNode* node) override {
        next.push_back(node);
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << "\"]" << std::endl;

        for (const auto& arg : next) {
            std::cout << "\t" << name << " -> " << arg->name << ";" << std::endl;
            arg->print();
        }
    }
};



class BlockNode : public AstNode {
private:
    std::vector<AstNode*> next;
public:
    BlockNode(const std::string& name) {
        this->name = name;
        this->label = "Block";
    }
    void add(AstNode* node) override {
        next.push_back(node);
    }
    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << "\"]" << std::endl;
        // std::vector<AstNode*>::iterator it;
        // for (it = next.begin(); it != next.end(); ++it) {
        //     std::cout << "\t" << name << " -> " << (*it)->name << ";" << std::endl;
        //     (*it)->print();
        // }
        for (const auto& stmt : next) {
            std::cout << "\t" << name << " -> " << stmt->name << ";" << std::endl;
            stmt->print();
        }
    }
    ~BlockNode() {
        for (const auto& stmt : next) {
            delete stmt;
        }
    }
};




class StatementsNode : public AstNode {
private:
    std::vector<AstNode*> next;

public:
    StatementsNode(const std::string& name) {
        this->name = name;
        this->label = "Block Statements";
    }

    void add(AstNode* node) override {
        next.push_back(node);
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << "\"]" << std::endl;
        for (const auto& stmt : next) {
            std::cout << "\t" << name << " -> " << stmt->name << ";" << std::endl;
            stmt->print();
        }
    }

    ~StatementsNode() {
        for (const auto& stmt : next) {
            delete stmt;
        }
    }
};


class assignmentStatement : public AstNode {
private:
    std::vector<AstNode*> next;

public:
    assignmentStatement(const std::string& name) {
        this->name = name;
        this->label = "assignment";
    }

    void add(AstNode* node) override {
        next.push_back(node);
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << "\"]" << std::endl;
        for (const auto& stmt : next) {
            std::cout << "\t" << name << " -> " << stmt->name << ";" << std::endl;
            stmt->print();
        }
        // for (const auto& stmt : next) {
        //     stmt->print();
        // }
    }

    ~assignmentStatement() {
        for (const auto& stmt : next) {
            delete stmt;
        }
    }
};

// Leaf node for representing numeric literals
class NumberNode : public AstNode {
private:
    int value;
public:
    NumberNode(std::string name, std::string label, int value) {
        this->name = name;
        this->label = label;
        this->value = value; 
    }

    void add(AstNode* /*node*/) override {
        std::cerr << "Cannot add a child to a leaf node." << std::endl;
    }

    void print() const override {
        std::cout << "\t" << name << " [shape=box,label=\"" << label << ": " << value << "\"]" << std::endl;

    }
};





// Composite node for representing binary expressions
class BinaryExpressionNode : public AstNode {
private:
    char operation;
    AstNode* left;
    AstNode* right;

public:
    BinaryExpressionNode(char op, AstNode* l, AstNode* r)
        : operation(op), left(l), right(r) {}

    void add(AstNode* node) override {
        if (!left)
            left = node;
        else if (!right)
            right = node;
        else
            std::cerr << "Binary expression already has two children." << std::endl;
    }

    void print() const override {
        std::cout << "\t" << "BinaryExpressionNode" << " [label=\"" << operation << "\"]" << std::endl;
        left->print();
        std::cout << "\t" << "BinaryExpressionNode" << " [label=\"" << operation << "\"]" << std::endl;
        right->print();
    }

    ~BinaryExpressionNode() {
        delete left;
        delete right;
    }
};


class ReturnStatementNode : public AstNode {
private:
    AstNode* returnValue;

public:
    ReturnStatementNode(AstNode* value)
        : returnValue(value) {
        this->name = "ReturnStatement";
    }

    void add(AstNode* /*node*/) override {
        std::cerr << "Cannot add a child to a leaf node." << std::endl;
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << "ReturnStatement" << "\"]" << std::endl;
        returnValue->print();
    }

    ~ReturnStatementNode() {
        delete returnValue;
    }
};


class AST {
private:
    AstNode* root = nullptr;
public:
    AST(AstNode* r) : root(r) {}

    ~AST() {
        if (root != nullptr) {
            delete root;
            root = nullptr;
        }
    }
    void Print() {
        std::cout << "digraph G {" << std::endl;
        root->print();
        std::cout << "}" << std::endl;
    }
};
#endif 
