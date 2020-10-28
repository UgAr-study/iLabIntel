#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <sys/stat.h>

enum Node_t     { BINOP, EXPR, FUNC, OPERATOR, VARNAME, BRACE, NUM, END };
// OPERATOR is {if / while}

enum BinOp_t    { ADD, SUB, MULT, DIV, ASSIGN };
enum Foo_t      { SCAN, PRINT };
enum Braces_t   { LBRACE, RBRACE };
enum Operator_t { WHILE, IF };

class Node;
class BinOp;
class Func;
class Expr;
class VarName;
class Num;
class Brace;

using VarTable = std::unordered_map<std::string, BinOp*>;


class Node {
    Node* parent;
    Node_t type;

public:
    Node (Node* parent_, Node_t type_)
        : parent (parent_), type (type_) {};
    Node_t getType () const { return type; }
};

class BinOp: public Node {
    BinOp_t operation;
    Node *lhs, *rhs;

public:
    explicit BinOp (BinOp_t type)
        : operation(type), Node(nullptr, BINOP)
        {lhs = nullptr; rhs = nullptr;};
    BinOp_t getOperation() const { return operation; }
    void setLhs (Node* Lhs) { lhs = Lhs; }
    void setRhs (Node* Rhs) { rhs = Rhs; }
    const Node* getLhs() const { return lhs; }
    const Node* getRhs() const { return rhs; }
};

class Expr: public Node {
    Node* top;

public:
    Expr(): Node(nullptr, EXPR) { top = nullptr; };
    Expr(std::vector<Node*>::iterator &cur_iter, VarTable& variables);
    int Culculate(std::unordered_map<std::string, int>& values) const;
    Node_t getTopType() const { return top->getType(); }
    void Dump() const;
    const Node* getTop() const { return top; }
};

class VarName: public Node {
    std::string name;

public:
    explicit VarName(std::string name_)
        : Node(nullptr, VARNAME), name(std::move(name_)) {};
    std::string getName () const { return name; }
};


class Func: public Node {
    Foo_t func;
public:
    Expr expression;

public:
    explicit Func(Foo_t type)
        : func (type), Node(nullptr, FUNC), expression() {};
    Foo_t getFunction () const { return func; }
    void setExpr (std::vector<Node*>::iterator &cur_iter, VarTable& variables) { expression = Expr{cur_iter, variables}; };
    const Expr& getExpression () const { return expression; }
};

class Num: public Node {
    int num;

public:
    Num(int number)
        : num (number), Node(nullptr, NUM) {};
    int getNum () const { return num; }
};

class Brace: public Node {
    Braces_t brace_type;

public:
    explicit Brace (Braces_t type)
        : brace_type(type), Node(nullptr, BRACE) {};
    Braces_t getBraceType () const { return brace_type; }
};

/*class END: public Node {

};*/

