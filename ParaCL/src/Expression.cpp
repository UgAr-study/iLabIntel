#include "../include/MainHeader.h"
#include "../include/LexemHeader.h"
#include "../include/ExpressionHeader.h"

Node* BuildSyntaxTree (std::vector<Node *>::iterator &cur_iter) {
    Node* tree;
    tree = Expression (cur_iter);

    /*
    if (end_of_expr != 1) {
        printf ("Not reach end of expression! Check braces\n");
        exit(0);
    }
     */

    if (tree == nullptr) {
        printf ("Nothing to be calculated\n");
        exit (0);
    }

    return tree;
}

Node* Expression (std::vector<Node *>::iterator &cur_iter) {
    Node *e_left;
    BinOp *expression;

    e_left = Multiplication (cur_iter);

    if (e_left != nullptr) {
        Node* cur_lex = *(++cur_iter);

        if (cur_lex->getType() == END) {
            delete cur_lex;
            return e_left;
        }

        while (IsAddSub(cur_lex)) {
            expression = static_cast<BinOp*>(cur_lex);
            expression->setLhs (e_left);
            ++cur_iter;

            Node* rhs = Multiplication (cur_iter);

            if (rhs == nullptr) {
                std::cout << "Wrong format: expected a number or expression\n";
                delete expression;
                return nullptr;
            }

            expression->setRhs (rhs);
            e_left = static_cast<Node*>(expression);
            cur_lex = *(++cur_iter);
        }
    }

    return e_left;
}

Node* Multiplication (std::vector<Node *>::iterator &cur_iter) {
    Node* m_left;
    BinOp* mult;

    m_left = Term (cur_iter);

    if (m_left != nullptr) {
        Node* cur_lex = *(++cur_iter);

        if (cur_lex->getType() == END) {
            delete cur_lex;
            return m_left;
        }

        while (IsMulDiv(cur_lex)) {
            mult = static_cast<BinOp*>(cur_lex);
            mult->setLhs(m_left);
            ++cur_iter;

            Node* rhs = Term (cur_iter);

            if (rhs == nullptr) {
                std::cout << "Wrong format: expected a number or expression\n";
                delete mult;
                return nullptr;
            }

            mult->setRhs (rhs);
            m_left = static_cast<Node*>(mult);
            cur_lex = *(++cur_iter);
        }
    }

    return m_left;
}

Node* Term (std::vector<Node *>::iterator &cur_iter) {
    Node* term = nullptr;
    Node* cur_lex = *cur_iter;

    Node_t type = cur_lex->getType();
    if (type == END)
        return nullptr;

    if (type == NUM || type == FUNC)
        return cur_lex;

    if (type == VARNAME) {

        //TODO add in unordered map
    }

    if (IsLBrace(cur_lex)) {
        term = Expression (++cur_iter);

        delete cur_lex;
        cur_lex = *(++cur_iter);

        if (!IsRBrace(cur_lex)) {
            std::cout << "Wrong format: expected ')'\n";
            delete term;
            delete cur_lex;
            return nullptr;
        }

        return term;
    } else {
        std::cout << "Wrong format: expected variable or brace\n";
        return nullptr;
    }
}

int TreeCalculator (Node* top) {
    if (top == nullptr) {
        std::cout << "Nothing to calculate\n";
        return 0;
    }

    if (top->getType() == NUM) {
        Num num = *((Num*) top);
        return num.getNum();
    }

    if (top->getType() == BINOP) {
        BinOp op = *((BinOp*) top);
        if (op.getOperation() == ASSIGN) {
            VarName var = *((VarName*) op.getLhs());
        }
    }
}

