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

/*
class FunctionCallNode : public AstNode {
private:
    std::vector<AstNode*> arguments;

public:
    FunctionCallNode(const std::string& functionName) {
        this->name = functionName;
        this->label = "Function Call";
    }

    void add(AstNode* arg) override {
        arguments.push_back(arg);
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label <<" : "<<name<<"\"]" << std::endl;
        for (const auto& arg : arguments) {
            std::cout << "\t" << name << " -> " << arg->name << ";" << std::endl;
            arg->print();
        }
    }

    ~FunctionCallNode() {
        for (const auto& arg : arguments) {
            delete arg;
        }
    }
};
*/
class WhileStatementNode : public AstNode {
private:
    AstNode* condition; // The condition to be evaluated
    AstNode* body;      // The body to be executed while the condition is true

public:
    WhileStatementNode(AstNode* cond, AstNode* bod)
        : condition(cond), body(bod) {
        this->name = "While";
        this->label = "While Statement";
    }

    // The add method is not typically used for while statements, but if needed,
    // it could be used to add additional nodes to the body, for example.
    void add(AstNode* node) override {
        // Implementation depends on the specific needs of your AST structure
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << name << "\"]" << std::endl;
        if (condition) {
            std::cout << "\t" << name << " -> " << condition->name << " [label=\"condition\"];" << std::endl;
            condition->print();
        }
        if (body) {
            std::cout << "\t" << name << " -> " << body->name << " [label=\"body\"];" << std::endl;
            body->print();
        }
    }

    ~WhileStatementNode() {
        delete condition;
        delete body;
    }
};


class ComparisonNode : public AstNode {
private:
    AstNode* leftExpression;
    std::string compOp;
    AstNode* rightExpression;

public:
    ComparisonNode(AstNode* left, const std::string& op, AstNode* right) {
        this->leftExpression = left;
        this->compOp = op;
        this->rightExpression = right;
        this->name = "Comparison";
        this->label = "Comparison";
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << compOp << "\"]" << std::endl;
        if (leftExpression) {
            std::cout << "\t" << name << " -> " << leftExpression->name << " [label=\"left\"];" << std::endl;
            leftExpression->print();
        }
        if (rightExpression) {
            std::cout << "\t" << name << " -> " << rightExpression->name << " [label=\"right\"];" << std::endl;
            rightExpression->print();
        }
    }


    ~ComparisonNode() {
        delete leftExpression;
        delete rightExpression;
    }
};

class PrimaryExpressionNode : public AstNode {
private:
    std::string value;

public:
    PrimaryExpressionNode(const std::string& val) {
        this->value = val;
        this->name = "PrimaryExpression";
        this->label = "Primary Expression";
    }

    // PrimaryExpressionNode does not typically have children, so the add method can be a no-op
    void add(AstNode* node) override {
        // No operation, as primary expressions do not have child nodes
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << value << "\"]" << std::endl;
    }
};

class NegatedExpressionNode : public AstNode {
private:
    AstNode* primaryExpression;

public:
    NegatedExpressionNode(AstNode* primary) {
        this->primaryExpression = primary;
        this->name = "NegatedExpression";
        this->label = "Negated Expression";
    }

    // NegatedExpressionNode does not typically have additional children, so the add method can be a no-op
    void add(AstNode* node) override {
        // No operation, as negated expressions do not have additional child nodes
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << "\"]" << std::endl;
        if (primaryExpression) { // Check if primaryExpression is not null
            std::cout << "\t" << name << " -> " << primaryExpression->name << ";" << std::endl;
            primaryExpression->print();
        }
    }

    ~NegatedExpressionNode() {
        delete primaryExpression;
    }
};

class ExpressionNode : public AstNode {
private:
    std::string op;
    AstNode* leftExpression;
    AstNode* rightExpression;

public:
    ExpressionNode(const std::string& op, AstNode* left, AstNode* right)
        : op(op), leftExpression(left), rightExpression(right) {
        this->name = "Expression";
        this->label = "Expression";
    }

    // ExpressionNode does not typically have additional children, so the add method can be a no-op
    void add(AstNode* node) override {
        // No operation, as expression nodes do not have additional child nodes
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << op << "\"]" << std::endl;
        if (leftExpression) { // Check if leftExpression is not null
            std::cout << "\t" << name << " -> " << leftExpression->name << ";" << std::endl;
            leftExpression->print();
        }
        if (rightExpression) { // Check if rightExpression is not null
            std::cout << "\t" << name << " -> " << rightExpression->name << ";" << std::endl;
            rightExpression->print();
        }
    }

    ~ExpressionNode() {
        delete leftExpression;
        delete rightExpression;
    }
};

class CompOpNode : public AstNode {
private:
    std::string op;

public:
    CompOpNode(const std::string& op) : op(op) {
        this->name = "CompOp";
        this->label = "Comp Op";
    }

    // CompOpNode does not have children, so the add method can be a no-op
    void add(AstNode* node) override {
        // No operation, as CompOp nodes do not have child nodes
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << op << "\"]" << std::endl;
    }
};

class ForStatementNode : public AstNode {
private:
    AstNode* forHeader;
    AstNode* changes;
    AstNode* block;

public:
    ForStatementNode(AstNode* header, AstNode* changes, AstNode* block)
        : forHeader(header), changes(changes), block(block) {
        this->name = "ForStatement";
        this->label = "For Statement";
    }

    // ForStatementNode does not typically have additional children, so the add method can be a no-op
    void add(AstNode* node) override {
        // No operation, as for statements do not have additional child nodes
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << name << "\"]" << std::endl;
        if (forHeader) {
            std::cout << "\t" << name << " -> " << forHeader->name << ";" << std::endl;
            forHeader->print();
        }
        if (changes) {
            std::cout << "\t" << name << " -> " << changes->name << ";" << std::endl;
            changes->print();
        }
        if (block) {
            std::cout << "\t" << name << " -> " << block->name << ";" << std::endl;
            block->print();
        }
    }

    ~ForStatementNode() {
        delete forHeader;
        delete changes;
        delete block;
    }
};

class ForHeaderNode : public AstNode {
private:
    std::string identifier;

public:
    ForHeaderNode(const std::string& id) {
        this->identifier = id;
        this->name = "ForHeader";
        this->label = "For Header";
    }

    // ForHeaderNode does not have children, so the add method can be a no-op
    void add(AstNode* node) override {
        // No operation, as ForHeader nodes do not have child nodes
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << identifier << "\"]" << std::endl;
    }
};
class ChangesNode : public AstNode {
private:
    std::string identifier;
    AstNode* range;

public:
    ChangesNode(const std::string& id) : identifier(id), range(nullptr) {
        this->name = "Changes";
        this->label = "Changes";
    }

    // Override the add method to set the range node
    void add(AstNode* node) override {
        if (!range) {
            range = node;
        }
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << identifier << "\"]" << std::endl;
        if (range) {
            std::cout << "\t" << name << " -> " << range->name << ";" << std::endl;
            range->print();
        }
    }

    ~ChangesNode() {
        delete range;
    }
};

class RangeNode : public AstNode {
private:
    std::vector<int> values;

public:
    RangeNode(const std::vector<int>& vals) : values(vals) {
        this->name = "Range";
        this->label = "Range";
    }

    // RangeNode does not have children, so the add method can be a no-op
    void add(AstNode* node) override {
        // No operation, as Range nodes do not have child nodes
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : ";
        for (size_t i = 0; i < values.size(); ++i) {
            std::cout << values[i];
            if (i < values.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\"]" << std::endl;
    }
};

class MyFuncNode : public AstNode {
private:
    std::string identifier;
    // If MyFuncNode is expected to have children, such as parameters or a body,
    // you should include a data structure to hold them, for example:
    // std::vector<AstNode*> children;

public:
    MyFuncNode(const std::string& id) : identifier(id) {
        this->name = "MyFunc";
        this->label = "My Func";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // If MyFuncNode is expected to have children, implement this method to add them to the children vector.
        // Otherwise, if MyFuncNode does not have children, this method can be a no-op.
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << identifier << "\"]" << std::endl;
        // If MyFuncNode has children, you should also print them here.
    }
};

class MyRangeNode : public AstNode {
private:
    std::vector<int> values;

public:
    MyRangeNode(const std::vector<int>& vals) : values(vals) {
        this->name = "MyRange";
        this->label = "My Range";
    }

    // MyRangeNode does not have children, so the add method can be a no-op
    void add(AstNode* node) override {
        // No operation, as MyRange nodes do not have child nodes
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : ";
        for (size_t i = 0; i < values.size(); ++i) {
            std::cout << values[i];
            if (i < values.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\"]" << std::endl;
    }
};

class TryStatementNode : public AstNode {
private:
    AstNode* block;
    AstNode* tryStmts;

public:
    TryStatementNode(AstNode* block, AstNode* tryStmts)
        : block(block), tryStmts(tryStmts) {
        this->name = "TryStatement";
        this->label = "Try Statement";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Implementation depends on the specific needs of your AST structure.
        // If TryStatementNode is expected to have more than one child node,
        // you should include a data structure to hold them, for example:
        // std::vector<AstNode*> children;
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << name << "\"]" << std::endl;
        if (block) { // Check if block is not null
            std::cout << "\t" << name << " -> " << block->name << ";" << std::endl;
            block->print();
        }
        if (tryStmts) { // Check if tryStmts is not null
            std::cout << "\t" << name << " -> " << tryStmts->name << ";" << std::endl;
            tryStmts->print();
        }
    }

    ~TryStatementNode() {
        delete block;
        delete tryStmts;
    }
};

class TryStmtsNode : public AstNode {
private:
    std::vector<AstNode*> tryStmts;

public:
    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        tryStmts.push_back(node);
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << name << "\"]" << std::endl;
        for (const auto& stmt : tryStmts) {
            std::cout << "\t" << name << " -> " << stmt->name << ";" << std::endl;
            stmt->print();
        }
    }

    ~TryStmtsNode() {
        for (auto& stmt : tryStmts) {
            delete stmt;
        }
    }
};

class ExceptBlockNode : public AstNode {
private:
    std::string identifier;
    AstNode* block;

public:
    ExceptBlockNode(const std::string& id, AstNode* block)
        : identifier(id), block(block) {
        this->name = "ExceptBlock";
        this->label = "Except Block";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since ExceptBlockNode represents an exception block with a single block of statements,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to an exception block, you could implement this method accordingly.
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << identifier << "\"]" << std::endl;
        if (block) { // Check if block is not null
            std::cout << "\t" << name << " -> " << block->name << ";" << std::endl;
            block->print();
        }
    }

    ~ExceptBlockNode() {
        delete block;
    }
};

class FinallyBlockNode : public AstNode {
private:
    AstNode* block;

public:
    FinallyBlockNode(AstNode* block) : block(block) {
        this->name = "FinallyBlock";
        this->label = "Finally Block";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since FinallyBlockNode represents a finally block with a single block of statements,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a finally block, you could implement this method accordingly.
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << name << "\"]" << std::endl;
        if (block) { // Check if block is not null
            std::cout << "\t" << name << " -> " << block->name << ";" << std::endl;
            block->print();
        }
    }

    ~FinallyBlockNode() {
        delete block;
    }
};


class DecoratorsNode : public AstNode {
private:
    std::vector<AstNode*> decorators;
    AstNode* namedExpression;

public:
    DecoratorsNode(AstNode* namedExpr) : namedExpression(namedExpr) {
        this->name = "Decorators";
        this->label = "Decorators";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        decorators.push_back(node);
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << name << "\"]" << std::endl;
        if (namedExpression) { // Check if namedExpression is not null
            std::cout << "\t" << name << " -> " << namedExpression->name << ";" << std::endl;
            namedExpression->print();
        }
        for (const auto& decorator : decorators) {
            std::cout << "\t" << name << " -> " << decorator->name << ";" << std::endl;
            decorator->print();
        }
    }

    ~DecoratorsNode() {
        delete namedExpression;
        for (auto& decorator : decorators) {
            delete decorator;
        }
    }
};

class ClassDefNode : public AstNode {
private:
    AstNode* decorators;
    AstNode* classDefRaw;

public:
    ClassDefNode(AstNode* decorators, AstNode* classDefRaw)
        : decorators(decorators), classDefRaw(classDefRaw) {
        this->name = "ClassDef";
        this->label = "Class Definition";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since ClassDefNode represents a class definition with decorators and a class body,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a class definition, you could implement this method accordingly.
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << name << "\"]" << std::endl;
        if (decorators) { // Check if decorators is not null
            std::cout << "\t" << name << " -> " << decorators->name << ";" << std::endl;
            decorators->print();
        }
        if (classDefRaw) { // Check if classDefRaw is not null
            std::cout << "\t" << name << " -> " << classDefRaw->name << ";" << std::endl;
            classDefRaw->print();
        }
    }

    ~ClassDefNode() {
        delete decorators;
        delete classDefRaw;
    }
};

class ClassDefRawNode : public AstNode {
private:
    std::string identifier;
    AstNode* block;

public:
    ClassDefRawNode(const std::string& id, AstNode* block)
        : identifier(id), block(block) {
        this->name = "ClassDefRaw";
        this->label = "Class Definition Raw";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since ClassDefRawNode represents a class definition with a single block of statements,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a class definition, you could implement this method accordingly.
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << identifier << "\"]" << std::endl;
        if (block) { // Check if block is not null
            std::cout << "\t" << name << " -> " << block->name << ";" << std::endl;
            block->print();
        }
    }

    ~ClassDefRawNode() {
        delete block;
    }
};

class NamedExpressionNode : public AstNode {
private:
    AstNode* expression;

public:
    NamedExpressionNode(AstNode* expr) : expression(expr) {
        this->name = "NamedExpression";
        this->label = "Named Expression";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since NamedExpressionNode represents a named expression with a single expression,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a named expression, you could implement this method accordingly.
    }

    void print() const override {
        if (expression) { // Check if expression is not null
            expression->print();
        }
    }

    ~NamedExpressionNode() {
        delete expression;
    }
};

class WithStmtNode : public AstNode {
private:
    std::vector<AstNode*> withItems;
    AstNode* block;

public:
    WithStmtNode(const std::vector<AstNode*>& items, AstNode* block)
        : withItems(items), block(block) {
        this->name = "WithStmt";
        this->label = "With Statement";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        withItems.push_back(node);
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << name << "\"]" << std::endl;
        for (const auto& item : withItems) {
            std::cout << "\t" << name << " -> " << item->name << ";" << std::endl;
            item->print();
        }
        if (block) { // Check if block is not null
            block->print();
        }
    }

    ~WithStmtNode() {
        for (auto& item : withItems) {
            delete item;
        }
        delete block;
    }
};

class WithItemsNode : public AstNode {
private:
    std::vector<AstNode*> withItemLists;

public:
    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        withItemLists.push_back(node);
    }

    void print() const override {
        for (const auto& itemList : withItemLists) {
            itemList->print();
        }
    }

    ~WithItemsNode() {
        for (auto& itemList : withItemLists) {
            delete itemList;
        }
    }
};

class WithItemList : public AstNode {
private:
    std::vector<AstNode*> withItems;

public:
    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        withItems.push_back(node);
    }

    void print() const override {
        for (const auto& item : withItems) {
            item->print();
        }
    }

    ~WithItemList() {
        for (auto& item : withItems) {
            delete item;
        }
    }
};

class WithItem : public AstNode {
private:
    std::string identifier1;
    std::string stringLiteral;
    std::string identifier2;

public:
    WithItem(const std::string& id1, const std::string& str, const std::string& id2)
        : identifier1(id1), stringLiteral(str), identifier2(id2) {
        this->name = "WithItem";
        this->label = "With Item";
    }

    // WithItem does not have children, so the add method can be a no-op
    void add(AstNode* node) override {
        // No operation, as WithItem nodes do not have child nodes
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << identifier1;
        if (!stringLiteral.empty()) {
            std::cout << " = " << stringLiteral;
        }
        if (!identifier2.empty()) {
            std::cout << " as " << identifier2;
        }
        std::cout << "\"]" << std::endl;
    }
};

class FunctionCallNode : public AstNode {
private:
    std::string identifier;
    std::vector<AstNode*> arguments;

public:
    FunctionCallNode(const std::string& id) : identifier(id) {}


    void add(AstNode* arg) override {
        arguments.push_back(arg);
    }
    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label <<" : "<<identifier<<"\"]" << std::endl;
        for (const auto& arg : arguments) {
            std::cout << "\t" << name << " -> " << arg->name << ";" << std::endl;
            arg->print();
        }
    }

    ~FunctionCallNode() {
        for (const auto& arg : arguments) {
            delete arg;
        }
    }
};

class ArgumentsNode : public AstNode {
private:
    std::vector<AstNode*> arguments;

public:
     void add(AstNode* arg) override {
        arguments.push_back(arg);
    }

    void print() const override {
        for (const auto& arg : arguments) {
            arg->print();
        }
    }

    ~ArgumentsNode() {
        for (const auto& arg : arguments) {
            delete arg;
        }
    }
};

class ArgumentNode : public AstNode {
private:
    AstNode* primaryExpression;

public:
    ArgumentNode(AstNode* expr) : primaryExpression(expr) {}


    
    void add(AstNode* node) override {
        // Since ArgumentNode represents a single expression, we might not need to add anything.
        // However, if your language allows for some kind of modification to a primary expression,
        // you could implement this method accordingly.
    }
    void print() const override {
        primaryExpression->print();
    }

    ~ArgumentNode() {
        delete primaryExpression;
    }
};



class GlobalStmtNode : public AstNode {
private:
    std::string identifier;
    std::vector<std::string> globalParams;

public:
    GlobalStmtNode(const std::string& id, const std::vector<std::string>& params)
        : identifier(id), globalParams(params) {
        this->name = "GlobalStmt";
        this->label = "Global Statement";
    }

    // GlobalStmtNode does not have children, so the add method can be a no-op
    void add(AstNode* node) override {
        // No operation, as GlobalStmt nodes do not have child nodes
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << identifier << "\"]" << std::endl;
        for (const auto& param : globalParams) {
            std::cout << "\t" << identifier << " -> " << param << ";" << std::endl;
        }
    }
};

class NonlocalStmtNode : public AstNode {
private:
    std::string identifier;
    std::vector<std::string> nonlocalParams;

public:
    NonlocalStmtNode(const std::string& id, const std::vector<std::string>& params)
        : identifier(id), nonlocalParams(params) {
        this->name = "NonlocalStmt";
        this->label = "Nonlocal Statement";
    }

    // NonlocalStmtNode does not have children, so the add method can be a no-op
    void add(AstNode* node) override {
        // No operation, as NonlocalStmt nodes do not have child nodes
    }

    void print() const override {
        std::cout << "\t" << name << " [label=\"" << label << " : " << identifier << "\"]" << std::endl;
        for (const auto& param : nonlocalParams) {
            std::cout << "\t" << identifier << " -> " << param << ";" << std::endl;
        }
    }
};


class YieldStmtNode : public AstNode {
private:
    AstNode* yieldExpr;

public:
    YieldStmtNode(AstNode* expr) : yieldExpr(expr) {
        this->name = "YieldStmt";
        this->label = "Yield Statement";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since YieldStmtNode represents a yield statement with a single expression,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a yield statement, you could implement this method accordingly.
    }

    void print() const override {
        if (yieldExpr) { // Check if yieldExpr is not null
            yieldExpr->print();
        }
    }

    ~YieldStmtNode() {
        delete yieldExpr;
    }
};

class YieldExprNode : public AstNode {
private:
    AstNode* expression;

public:
    YieldExprNode(AstNode* expr) : expression(expr) {
        this->name = "YieldExpr";
        this->label = "Yield Expression";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since YieldExprNode represents a yield expression with a single expression,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a yield expression, you could implement this method accordingly.
    }

    void print() const override {
        if (expression) { // Check if expression is not null
            expression->print();
        }
    }

    ~YieldExprNode() {
        delete expression;
    }
};

class IfStatementNode : public AstNode {
private:
    AstNode* ifHeader;
    AstNode* block;
    AstNode* elifElse;

public:
    IfStatementNode(AstNode* header, AstNode* block, AstNode* elifElse)
        : ifHeader(header), block(block), elifElse(elifElse) {
        this->name = "IfStatement";
        this->label = "If Statement";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since IfStatementNode represents an if statement with a header, block, and optional elif/else,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to an if statement, you could implement this method accordingly.
    }

    void print() const override {
        if (ifHeader) { // Check if ifHeader is not null
            ifHeader->print();
        }
        if (block) { // Check if block is not null
            block->print();
        }
        if (elifElse) { // Check if elifElse is not null
            elifElse->print();
        }
    }

    ~IfStatementNode() {
        delete ifHeader;
        delete block;
        delete elifElse;
    }
};

class IfHeaderNode : public AstNode {
private:
    AstNode* namedExpression;

public:
    IfHeaderNode(AstNode* expr) : namedExpression(expr) {
        this->name = "IfHeader";
        this->label = "If Header";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since IfHeaderNode represents an if header with a single expression,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to an if header, you could implement this method accordingly.
    }

    void print() const override {
        if (namedExpression) { // Check if namedExpression is not null
            namedExpression->print();
        }
    }

    ~IfHeaderNode() {
        delete namedExpression;
    }
};

class ElifElseNode : public AstNode {
private:
    std::vector<AstNode*> elifStmts;
    AstNode* elseStmt;

public:
    ElifElseNode(const std::vector<AstNode*>& elifStmts, AstNode* elseStmt)
        : elifStmts(elifStmts), elseStmt(elseStmt) {
        this->name = "ElifElse";
        this->label = "Elif/Else";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        elifStmts.push_back(node);
    }

    void print() const override {
        for (const auto& stmt : elifStmts) {
            stmt->print();
        }
        if (elseStmt) { // Check if elseStmt is not null
            elseStmt->print();
        }
    }

    ~ElifElseNode() {
        for (auto& stmt : elifStmts) {
            delete stmt;
        }
        delete elseStmt;
    }
};

class ElifStmtsNode : public AstNode {
private:
    std::vector<AstNode*> elifStmts;

public:
    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        elifStmts.push_back(node);
    }

    void print() const override {
        for (const auto& stmt : elifStmts) {
            stmt->print();
        }
    }

    ~ElifStmtsNode() {
        for (auto& stmt : elifStmts) {
            delete stmt;
        }
    }
};


class ElifStmtNode : public AstNode {
private:
    AstNode* elifHeader;
    AstNode* block;

public:
    ElifStmtNode(AstNode* header, AstNode* block) : elifHeader(header), block(block) {
        this->name = "ElifStmt";
        this->label = "Elif Statement";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since ElifStmtNode represents an elif statement with a header and block,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to an elif statement, you could implement this method accordingly.
    }

    void print() const override {
        if (elifHeader) { // Check if elifHeader is not null
            elifHeader->print();
        }
        if (block) { // Check if block is not null
            block->print();
        }
    }

    ~ElifStmtNode() {
        delete elifHeader;
        delete block;
    }
};

class ElifHeaderNode : public AstNode {
private:
    AstNode* namedExpression;

public:
    ElifHeaderNode(AstNode* expr) : namedExpression(expr) {
        this->name = "ElifHeader";
        this->label = "Elif Header";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since ElifHeaderNode represents an elif header with a single expression,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to an elif header, you could implement this method accordingly.
    }

    void print() const override {
        if (namedExpression) { // Check if namedExpression is not null
            namedExpression->print();
        }
    }

    ~ElifHeaderNode() {
        delete namedExpression;
    }
};

class ElseStmtNode : public AstNode {
private:
    AstNode* block;

public:
    ElseStmtNode(AstNode* blk) : block(blk) {
        this->name = "ElseStmt";
        this->label = "Else Statement";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since ElseStmtNode represents an else statement with a single block,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to an else statement, you could implement this method accordingly.
    }

    void print() const override {
        if (block) { // Check if block is not null
            block->print();
        }
    }

    ~ElseStmtNode() {
        delete block;
    }
};

class MatchStmtNode : public AstNode {
private:
    AstNode* expression;
    AstNode* matchCases;

public:
    MatchStmtNode(AstNode* expr, AstNode* cases) : expression(expr), matchCases(cases) {
        this->name = "MatchStmt";
        this->label = "Match Statement";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since MatchStmtNode represents a match statement with an expression and cases,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a match statement, you could implement this method accordingly.
    }

    void print() const override {
        if (expression) { // Check if expression is not null
            expression->print();
        }
        if (matchCases) { // Check if matchCases is not null
            matchCases->print();
        }
    }

    ~MatchStmtNode() {
        delete expression;
        delete matchCases;
    }
};

class MatchCasesNode : public AstNode {
private:
    std::vector<AstNode*> matchCases;

public:
    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        matchCases.push_back(node);
    }

    void print() const override {
        for (const auto& matchCase : matchCases) {
            matchCase->print();
        }
    }

    ~MatchCasesNode() {
        for (auto& matchCase : matchCases) {
            delete matchCase;
        }
    }
};

class MatchCaseNode : public AstNode {
private:
    AstNode* patternList;
    AstNode* simpleStmt;

public:
    MatchCaseNode(AstNode* patternList, AstNode* simpleStmt) : patternList(patternList), simpleStmt(simpleStmt) {
        this->name = "MatchCase";
        this->label = "Match Case";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since MatchCaseNode represents a match case with a pattern list and a simple statement,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a match case, you could implement this method accordingly.
    }

    void print() const override {
        if (patternList) { // Check if patternList is not null
            patternList->print();
        }
        if (simpleStmt) { // Check if simpleStmt is not null
            simpleStmt->print();
        }
    }

    ~MatchCaseNode() {
        delete patternList;
        delete simpleStmt;
    }
};

class PatternListNode : public AstNode {
private:
    std::vector<AstNode*> patterns;

public:
    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        patterns.push_back(node);
    }

    void print() const override {
        for (const auto& pattern : patterns) {
            pattern->print();
        }
    }

    ~PatternListNode() {
        for (auto& pattern : patterns) {
            delete pattern;
        }
    }
};

class PatternNode : public AstNode {
private:
    AstNode* expression;  // Or other specific pattern nodes

public:
    PatternNode(AstNode* expr) : expression(expr) {
        this->name = "Pattern";
        this->label = "Pattern";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since PatternNode represents a pattern with a single expression,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a pattern, you could implement this method accordingly.
    }

    void print() const override {
        if (expression) { // Check if expression is not null
            expression->print();
        }
    }

    ~PatternNode() {
        delete expression;
    }
};

class ListPatternNode : public AstNode {
private:
    AstNode* patternList;

public:
    ListPatternNode(AstNode* list) : patternList(list) {
        this->name = "ListPattern";
        this->label = "List Pattern";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since ListPatternNode represents a list pattern with a single pattern list,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a list pattern, you could implement this method accordingly.
    }

    void print() const override {
        if (patternList) { // Check if patternList is not null
            patternList->print();
        }
    }

    ~ListPatternNode() {
        delete patternList;
    }
};

class DictPatternNode : public AstNode {
private:
    AstNode* dictPatternEntries;

public:
    DictPatternNode(AstNode* entries) : dictPatternEntries(entries) {
        this->name = "DictPattern";
        this->label = "Dictionary Pattern";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since DictPatternNode represents a dictionary pattern with entries,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a dictionary pattern, you could implement this method accordingly.
    }

    void print() const override {
        if (dictPatternEntries) { // Check if dictPatternEntries is not null
            dictPatternEntries->print();
        }
    }

    ~DictPatternNode() {
        delete dictPatternEntries;
    }
};

class DictPatternEntriesNode : public AstNode {
private:
    std::vector<AstNode*> dictPatternEntries;

public:
    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        dictPatternEntries.push_back(node);
    }

    void print() const override {
        for (const auto& entry : dictPatternEntries) {
            entry->print();
        }
    }

    ~DictPatternEntriesNode() {
        for (auto& entry : dictPatternEntries) {
            delete entry;
        }
    }
};

class DictPatternEntryNode : public AstNode {
private:
    AstNode* key;
    AstNode* value;

public:
    DictPatternEntryNode(AstNode* key, AstNode* value) : key(key), value(value) {
        this->name = "DictPatternEntry";
        this->label = "Dictionary Pattern Entry";
    }

    // Override the add method to handle child nodes
    void add(AstNode* node) override {
        // Since DictPatternEntryNode represents a dictionary pattern entry with a key and value,
        // we might not need to add anything. However, if your language allows for some kind
        // of modification to a dictionary pattern entry, you could implement this method accordingly.
    }

    void print() const override {
        if (key) { // Check if key is not null
            key->print();
        }
        if (value) { // Check if value is not null
            value->print();
        }
    }

    ~DictPatternEntryNode() {
        delete key;
        delete value;
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

class LiteralNode : public AstNode {
private:
    int value;
public:
    LiteralNode(std::string name, std::string label, int value) {
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
