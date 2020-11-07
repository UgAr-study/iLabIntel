#include "../include/ExpressionHeader.h"

Node::~Node() noexcept {}

int Expr::Culculate(VarValues & values) const {
    return TreeCalculator (top, values);
}

Expr::Expr(std::vector<Node *>::iterator &cur_iter, VarValues& values, int l_num) : Node(EXPR, (*cur_iter)->getLineNumber()){
    top = BuildSyntaxTree(cur_iter, values);
}

void Expr::Dump() const {
    PrintTree(top, 0);
}


Condition::Condition(std::vector<Node *>::iterator &cur_iter, std::vector<Node *>::iterator end, VarValues &values) {
    Node* first_lex = *cur_iter;

    if (cur_iter == end)
        return;

    if (first_lex->getType() != BRACE) {
        std::cout << "Error: line " << first_lex->getLineNumber() << ": expected LROUNDBRACK for condition" << std::endl;
        return;
    }

    Brace open_brace_for_scope = *(static_cast<Brace*>(first_lex));
    if (open_brace_for_scope.getBraceType() != LROUNDBRACK) {
        std::cout << "Error: line " << open_brace_for_scope.getLineNumber() << ": expected LROUNDBRACK for condition" << std::endl;
        return;
    }

    int number_of_opened_brackets = 1;
    std::vector<Node*> condition_nodes;
    Node *cmp_op = nullptr;


    while (number_of_opened_brackets > 0) {

        Node* cur_lex = *(++cur_iter);

        if (cur_iter == end)
            return;

        Node_t type = cur_lex->getType();

        if (type == BRACE) {
            auto brace = static_cast<const Brace*>(cur_lex);

            if (brace->getBraceType() == LROUNDBRACK)
                number_of_opened_brackets++;

            if (brace->getBraceType() == RROUNDBRACK)
                number_of_opened_brackets--;
        }

        if (type == END) {
            std::cout << "Error: line " << cur_lex->getLineNumber() <<": end in the condition" << std::endl;
            return;
        }

        if (type == BINOP) {
            auto binOp = static_cast<const BinOp*>(cur_lex);

            if (binOp->isCompare()) {
                condition_nodes.push_back(new End{binOp->getLineNumber()});
                cmp_op = cur_lex;
            }
        }

        condition_nodes.push_back(cur_lex);
    }

    condition_nodes.pop_back();
    condition_nodes.push_back (new End{(*cur_iter)->getLineNumber()});

    b_op = static_cast<BinOp*>(cmp_op);

    auto lhs_begin = condition_nodes.begin();
    Expr* left_expr = new Expr{lhs_begin, values, (*lhs_begin)->getLineNumber()};

    auto rhs_begin = lhs_begin + 3;
    Expr* right_expr = new Expr{rhs_begin, values, (*rhs_begin)->getLineNumber()};

    b_op->setLhs(static_cast<Node*>(left_expr));
    b_op->setRhs(static_cast<Node*>(right_expr));
}

bool Condition::isTrue(VarValues &GlobalValues) const {
    int left_res = (static_cast<const Expr*>(b_op->getLhs()))->Culculate(GlobalValues);
    int right_res = (static_cast<const Expr*>(b_op->getRhs()))->Culculate(GlobalValues);

    switch (b_op->getOperation()) {

        case EQUAL:
            return (left_res == right_res);
        case NOTEQUAL:
            return (left_res != right_res);
        case LESS:
            return (left_res < right_res);
        case OVER:
            return (left_res > right_res);
        case LESSEQUAL:
            return (left_res <= right_res);
        case OVEREQUAL:
            return (left_res >= right_res);
        default:
            std::cout << "Error: line " << b_op->getLineNumber() << ": expected comparing operation in condition" << std::endl;
            return false;
    }
}

bool BinOp::isCompare() const {

    if (operation == EQUAL || operation == NOTEQUAL ||
        operation == LESS  || operation == OVER ||
        operation == LESSEQUAL || operation == OVEREQUAL)
        return true;

    return false;
}

Scope::Scope(std::vector<Node *>::iterator &cur_iter, std::vector<Node *>::iterator end_code, VarValues &values) {
    Node* first_lex = *cur_iter;
    if (cur_iter == end_code)
        return;

    if (first_lex->getType() != BRACE) {
        std::cout << "Error: line " << first_lex->getLineNumber() << ": expected OPENBRACE for scope" << std::endl;
        return;
    }

    auto open_brace_for_scope = static_cast<const Brace*>(first_lex);
    if (open_brace_for_scope->getBraceType() != OPENBRACE) {
        std::cout << "Error: line " << open_brace_for_scope->getLineNumber() << ": expected OPENBRACE for scope" << std::endl;
        return;
    }

    int number_of_opened_braces = 1;
    begin = cur_iter + 1;

    while (number_of_opened_braces > 0) {
        Node* cur_lex = *(++cur_iter);

        if (cur_iter == end_code)
            return;

        if (cur_lex->getType() == BRACE) {
            auto brace = static_cast<const Brace*>(cur_lex);

            if (brace->getBraceType() == OPENBRACE)
                number_of_opened_braces++;

            if (brace->getBraceType() == CLOSEBRACE)
                number_of_opened_braces--;
        }
    }

    end = cur_iter;
    valid = true;
}

Branch_Operator::Branch_Operator(Branch_Operator_t type,
                                 std::vector<Node *>::iterator &cur_iter,
                                 std::vector<Node *>::iterator end,
                                 VarValues &values)
                                 : op_type(type), Node(BRANCHOPERATOR, (*cur_iter)->getLineNumber()) {

    Node* condition_start = *cur_iter;

    auto condition_ = new Condition{cur_iter, end, values};

    if (!condition_->isValid()) {
        std::cout << "Error: line " << condition_start->getLineNumber() << ": condition is not valid" << std::endl;
        return;
    }

    setCondition(condition_);

    Node* scope_start = *cur_iter;
    auto scope_ = new Scope {(++cur_iter), end, values};

    if (!scope_->isValid()) {
        std::cout << "Error: line " << scope_start->getLineNumber() << ": scop is not valid" << std::endl;
        return;
    }

    setScope(scope_);
}
