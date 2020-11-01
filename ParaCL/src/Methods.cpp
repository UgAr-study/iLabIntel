#include "../include/ExpressionHeader.h"
#include "../include/MainHeader.h"

//Node::~Node() noexcept {}

int Expr::Culculate(VarValues & values) const {
    return TreeCalculator (top, values);
}

Expr::Expr(std::vector<Node *>::iterator &cur_iter, VarValues& values ) : Node(EXPR){
    top = BuildSyntaxTree(cur_iter, values);
}

void Expr::Dump() const {
    PrintTree(top, 0);
}


Condition::Condition(std::vector<Node *>::iterator &cur_iter, VarValues &values) {
    Node* first_lex = *cur_iter;

    if (first_lex->getType() != BRACE) {
        std::cout << "Error: expected LROUNDBRACK for condition" << std::endl;
        return;
    }

    Brace open_brace_for_scope = *(static_cast<Brace*>(first_lex));
    if (open_brace_for_scope.getBraceType() != LROUNDBRACK) {
        std::cout << "Error: expected LROUNDBRACK for condition" << std::endl;
        return;
    }

    int number_of_opened_brackets = 1;
    std::vector<Node*> condition_nodes;
    Node *cmp_op = nullptr;


    while (number_of_opened_brackets > 0) {

        Node* cur_lex = *(++cur_iter);
        Node_t type = cur_lex->getType();

        if (type == BRACE) {
            Brace brace = *(static_cast<Brace*>(cur_lex));

            if (brace.getBraceType() == LROUNDBRACK)
                number_of_opened_brackets++;

            if (brace.getBraceType() == RROUNDBRACK)
                number_of_opened_brackets--;
        }

        if (type == END) {
            std::cout << "Error: End in the condition" << std::endl;
            return;
        }

        if (type == BINOP) {
            BinOp binOp = *(static_cast<BinOp*>(cur_lex));

            if (binOp.isCompare()) {
                condition_nodes.push_back(new End{});
                cmp_op = cur_lex;
            }
        }

        condition_nodes.push_back(cur_lex);
    }

    condition_nodes.pop_back();
    condition_nodes.push_back (new End{});

    b_op = static_cast<BinOp*>(cmp_op);

    auto lhs_begin = condition_nodes.begin();
    Expr* left_expr = new Expr{lhs_begin, values};

    auto rhs_begin = lhs_begin + 3;
    Expr* right_expr = new Expr{rhs_begin, values};

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
            std::cout << "Error: expected comparing operation in condition" << std::endl;
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

Scope::Scope(std::vector<Node *>::iterator &cur_iter, VarValues &values) {
    Node* first_lex = *cur_iter;

    if (first_lex->getType() != BRACE) {
        std::cout << "Error: expected OPENBRACE for scope" << std::endl;
        return;
    }

    Brace open_brace_for_scope = *(static_cast<Brace*>(first_lex));
    if (open_brace_for_scope.getBraceType() != OPENBRACE) {
        std::cout << "Error: expected OPENBRACE for scope" << std::endl;
        return;
    }

    int number_of_opened_braces = 1;

    while (number_of_opened_braces > 0) {
        Node* cur_lex = *(++cur_iter);

        if (cur_lex->getType() == BRACE) {
            Brace brace = *(static_cast<Brace*>(cur_lex));

            if (brace.getBraceType() == OPENBRACE)
                number_of_opened_braces++;

            if (brace.getBraceType() == CLOSEBRACE)
                number_of_opened_braces--;
        }

        scope_code.push_back(cur_lex);
    }

    scope_code.pop_back();
}

Branch_Operator::Branch_Operator(Branch_Operator_t type,
                                 std::vector<Node *>::iterator &cur_iter,
                                 VarValues &values)
                                 : op_type(type), Node(BRANCHOPERATOR) {

    auto condition_ = new Condition{cur_iter, values};

    if (!condition_->isValid()) {
        std::cout << "Error: condition is not valid" << std::endl;
        return;
    }

    setCondition(condition_);

    auto scope_ = new Scope {(++cur_iter), values};

    if (!scope_->isValid()) {
        std::cout << "Error: scop is not valid" << std::endl;
        return;
    }

    setScope(scope_);
}
