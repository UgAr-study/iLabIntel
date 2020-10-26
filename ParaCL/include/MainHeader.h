#pragma once

#include <iostream>
#include <utility>
#include <vector>

enum Node_t     { BINOP, EXPR, FUNC, OPERATOR, VARNAME, BRACE, NUM, END };
// OPERATOR is {if / while}

enum BinOp_t    { ADD, SUB, MULT, DIV, ASSIGN };
enum Foo_t      { SCAN, PRINT };
enum Braces_t   { LBRACE, RBRACE };
enum Operator_t { WHILE, IF };

class Node {
    Node* parent;
    Node_t type;

public:
    Node (Node* parent_, Node_t type_)
        : parent (parent_), type (type_) {};
    Node_t getType () { return type; }
};

class BinOp: public Node {
    BinOp_t operation;
    Node *lhs, *rhs;

public:
    explicit BinOp (BinOp_t type)
        : operation(type), Node(nullptr, BINOP)
        {lhs = nullptr; rhs = nullptr;};
    BinOp_t getOperation() { return operation; }
    void setLhs (Node* Lhs) { lhs = Lhs; }
    void setRhs (Node* Rhs) { rhs = Rhs; }
    const Node* getLhs() { return lhs; }
    const Node* getRhs() { return rhs; }
};

class Expr: public Node {
    Node* top;

public:
    Expr();
    Expr(std::vector<Node*>::iterator &cur_iter);
    int Culculate();
};

class VarName: public Node {
    std::string name;

public:
    explicit VarName(std::string name_)
        : Node(nullptr, VARNAME), name(std::move(name_)) {};
    std::string getName () { return name; }
};


class Func: public Node {
    Foo_t func;
    Expr expression;

public:
    explicit Func(Foo_t type)
        : func (type), Node(nullptr, FUNC) {};
    Foo_t getFunction () { return func; }
    Expr getExpression () const {}
};

class Num: public Node {
    int num;

public:
    Num(int number)
        : num (number), Node(nullptr, NUM) {};
    int getNum () { return num; }
};

class Brace: public Node {
    Braces_t brace_type;

public:
    explicit Brace (Braces_t type)
        : brace_type(type), Node(nullptr, BRACE) {};
    Braces_t getBraceType () { return brace_type; }
};

/*class END: public Node {

};*/

