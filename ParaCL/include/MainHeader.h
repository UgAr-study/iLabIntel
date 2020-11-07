#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <sys/stat.h>


enum Node_t     { BINOP, EXPR, FUNC, BRANCHOPERATOR, CONDITION, SCOPE, VARNAME, BRACE, NUM, END };
enum BinOp_t    { ADD, SUB, MULT, DIV, ASSIGN, EQUAL, NOTEQUAL, LESS, OVER, LESSEQUAL, OVEREQUAL };
enum Foo_t      { SCAN, PRINT };
enum Braces_t   { LROUNDBRACK, RROUNDBRACK, OPENBRACE, CLOSEBRACE };
enum Branch_Operator_t { WHILE, IF };

class Node;
class BinOp;
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
    int line_number = 0;

public:
    Node (Node_t type_, int l_num)
        : type (type_), line_number(l_num) {};
    Node_t getType () const { return type; }
    int getLineNumber() const { return line_number; }
    virtual ~Node() = 0;
};

class End: public Node {
public:
    End(int l_num): Node(END, l_num) {}
    ~End() override = default;
};

class BinOp: public Node {
    BinOp_t operation;
    Node *lhs = nullptr, *rhs = nullptr;

public:
    explicit BinOp (BinOp_t type, int l_num)
        : operation(type), Node(BINOP, l_num) {};

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
    Expr(int l_num): Node(EXPR, l_num) { top = nullptr; };
    Expr(std::vector<Node*>::iterator &cur_iter, VarValues& values, int l_num);
    int Culculate(VarValues & values) const;
    Node_t getTopType() const { return top->getType(); }
    void Dump() const;
    const Node* getTop() const { return top; }

    ~Expr() override { delete top; }
};

class VarName: public Node {
    std::string name;

public:
    explicit VarName(std::string name_, int l_num)
        : Node(VARNAME, l_num), name(std::move(name_)) {};
    std::string getName () const { return name; }

    ~VarName() override = default;
};


class Func: public Node {
    Foo_t func;
public:
    Expr *expression;

public:
    explicit Func(Foo_t type, int l_num)
        : func (type), Node(FUNC, l_num) {};
    Foo_t getFunction () const { return func; }
    void setExpr (std::vector<Node*>::iterator &cur_iter, VarValues &values) { expression = new Expr{cur_iter, values, (*cur_iter)->getLineNumber()}; };
    int CulcExpression(VarValues & values) { return expression->Culculate(values); }
    ~Func() override { delete expression; };
};

class Num: public Node {
    int num;

public:
    Num(int number, int l_num)
        : num (number), Node(NUM, l_num) {};
    int getNum () const { return num; }

    ~Num() override = default;
};

class Brace: public Node {
    Braces_t brace_type;

public:
    explicit Brace (Braces_t type, int l_num)
        : brace_type(type), Node(BRACE, l_num) {};
    Braces_t getBraceType () const { return brace_type; }

    ~Brace() override = default;
};

class Condition {
    BinOp *b_op = nullptr;
public:
    Condition(std::vector<Node*>::iterator &cur_iter, std::vector<Node *>::iterator end, VarValues &values);
    bool isTrue(VarValues& GlobalValues) const;
    bool isValid() { return b_op != nullptr; }

    ~Condition() = default;
};

class Scope {
    bool valid = false;
    std::vector<Node*>::iterator begin;
    std::vector<Node*>::iterator end;

public:
    Scope(std::vector<Node*>::iterator &cur_iter, std::vector<Node *>::iterator end, VarValues &values);
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
    explicit Branch_Operator(Branch_Operator_t type, int l_num) : op_type(type), Node(BRANCHOPERATOR, l_num) {}
    explicit Branch_Operator(Branch_Operator_t type,
                             std::vector<Node*>::iterator &cur_iter,
                             std::vector<Node *>::iterator end,
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



