#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <sys/stat.h>


enum Node_t     { BINOP, UNOP, EXPR, FUNC, BRANCHOPERATOR, CONDITION, SCOPE, VARNAME, BRACE, NUM, END };
enum BinOp_t    { ADD, SUB, MULT, DIV, ASSIGN, EQUAL, NOTEQUAL, LESS, OVER, LESSEQUAL, OVEREQUAL };
enum UnOp_t     { UNARYMINUS };
enum Foo_t      { SCAN, PRINT };
enum Braces_t   { LROUNDBRACK, RROUNDBRACK, OPENBRACE, CLOSEBRACE };
enum Branch_Operator_t { WHILE, IF };

class Node;
class BinOp;
class UnOp;
class Func;
class Expr;
class VarName;
class Num;
class Brace;
class Branch_Operator;
class Scope;
class Condition;

using VarValues = std::unordered_map<std::string, Num*>;

class Node {
    Node_t type;

public:
    Node (Node_t type_)
        : type (type_) {};
    Node_t getType () const { return type; }
    virtual ~Node() = 0;
};

class End: public Node {
public:
    End(): Node(END) {}
    ~End() override = default;
};

class UnOp: public Node {
    UnOp_t operation;
    Node *rhs;

public:
    explicit UnOp(UnOp_t type) : operation(type), Node(UNOP) {}

    UnOp_t getOperation() const { return operation; }
    void setRhs (Node* Rhs) { rhs = Rhs; }
    const Node* getRhs() const { return rhs; }

    ~UnOp() override { delete rhs; }
};

class BinOp: public Node {
    BinOp_t operation;
    Node *lhs = nullptr, *rhs = nullptr;

public:
    explicit BinOp (BinOp_t type)
        : operation(type), Node(BINOP) {};

    BinOp_t getOperation() const { return operation; }
    void setLhs (Node* Lhs) { lhs = Lhs; }
    void setRhs (Node* Rhs) { rhs = Rhs; }
    const Node* getLhs() const { return lhs; }
    const Node* getRhs() const { return rhs; }
    bool isCompare() const;

    ~BinOp() override { delete lhs; delete rhs; }
};

class Expr: public Node {
    Node* top;

public:
    Expr(): Node(EXPR) { top = nullptr; };
    Expr(std::vector<Node*>::iterator &cur_iter, VarValues& values);
    int Culculate(VarValues & values) const;
    Node_t getTopType() const { return top->getType(); }
    void Dump() const;
    const Node* getTop() const { return top; }

    ~Expr() override { delete top; }
};

class VarName: public Node {
    std::string name;

public:
    explicit VarName(std::string name_)
        : Node(VARNAME), name(std::move(name_)) {};
    std::string getName () const { return name; }

    ~VarName() override = default;
};


class Func: public Node {
    Foo_t func;
public:
    Expr *expression;

public:
    explicit Func(Foo_t type)
        : func (type), Node(FUNC) {};
    Foo_t getFunction () const { return func; }
    void setExpr (std::vector<Node*>::iterator &cur_iter, VarValues &values) { expression = new Expr{cur_iter, values}; };
    int CulcExpression(VarValues & values) { return expression->Culculate(values); }
    ~Func() override { delete expression; };
};

class Num: public Node {
    int num;

public:
    Num(int number)
        : num (number), Node(NUM) {};
    int getNum () const { return num; }

    ~Num() override = default;
};

class Brace: public Node {
    Braces_t brace_type;

public:
    explicit Brace (Braces_t type)
        : brace_type(type), Node(BRACE) {};
    Braces_t getBraceType () const { return brace_type; }

    ~Brace() override = default;
};

class Condition {
    BinOp *b_op = nullptr;
public:
    Condition(std::vector<Node*>::iterator &cur_iter, VarValues &values);
    bool isTrue(VarValues& GlobalValues) const;
    bool isValid() { return b_op != nullptr; }

    ~Condition() = default;
};

class Scope {
    bool valid = false;
    std::vector<Node*>::iterator begin;
    std::vector<Node*>::iterator end;

public:
    Scope(std::vector<Node*>::iterator &cur_iter, VarValues &values);
    std::vector<Node*>::iterator getBeginScopeCode() const { return begin; };
    std::vector<Node*>::iterator getEndScopeCode() const { return end; };

    bool isValid () { return valid; }

    ~Scope() = default;
};

class Branch_Operator: public Node {
    Branch_Operator_t op_type;
    Condition *condition = nullptr;
    Scope *scope = nullptr;

public:
    explicit Branch_Operator(Branch_Operator_t type) : op_type(type), Node(BRANCHOPERATOR) {}
    explicit Branch_Operator(Branch_Operator_t type,
                             std::vector<Node*>::iterator &cur_iter,
                             VarValues & values);

    Branch_Operator_t getOperatorType () const { return op_type; }
    void setCondition (Condition* condition_) { condition = condition_; };
    void setScope (Scope* scope_) { scope = scope_; }
    bool isConditionTrue (VarValues & GlobalValues) {
        return condition->isTrue(GlobalValues); };
    std::vector<Node*>::iterator getBeginScope() const { return scope->getBeginScopeCode(); }
    std::vector<Node*>::iterator getEndScope() const { return scope->getEndScopeCode(); }
    bool isValid () { return ( (condition != nullptr && scope != nullptr) &&
                               (condition->isValid() && scope->isValid())); }

    ~Branch_Operator() override { delete condition; delete scope; }
};



