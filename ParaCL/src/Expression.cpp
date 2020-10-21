#include "../include/MainHeader.h"
#include "../include/LexemHeader.h"
#include "../include/ExpressionHeader.h"

Node* BuildSyntaxTree (std::vector<char>& text, unsigned& cur_pos) {
    Node* tree;
    tree = Expression (text, cur_pos);

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

Node* Expression (std::vector<char>& text, unsigned& cur_pos) {
    Node *e_left;
    BinOp *expression;

    e_left = Multiplication (text, cur_pos);

    if (e_left != nullptr) {
        ++cur_pos;
        Node* cur_lex = GetLexem(text, cur_pos);

        if (cur_lex->getType() == END) {
            delete cur_lex;
            return e_left;
        }

        while (IsAddSub(cur_lex)) {
            expression = (BinOp*) cur_lex;
            expression->setLhs (e_left);
            ++cur_pos;

            Node* rhs = Multiplication (text, cur_pos);

            if (rhs == nullptr) {
                std::cout << "Wrong format: expected a number or expression\n";
                delete expression;
                return nullptr;
            }

            expression->setRhs (rhs);
            e_left = (Node*) expression;
            ++cur_pos;
            cur_lex = GetLexem(text, cur_pos);
        }
    }

    return e_left;
}

Node* Multiplication (std::vector<char>& text, unsigned& cur_pos) {
    Node* m_left;
    BinOp* mult;

    m_left = Term (text, cur_pos);

    if (m_left != nullptr) {
        ++cur_pos;
        Node* cur_lex = GetLexem (text, cur_pos);

        if (cur_lex->getType() == END) {
            delete cur_lex;
            return m_left;
        }

        while (IsMulDiv(cur_lex)) {
            mult = (BinOp*) cur_lex;
            mult->setLhs(m_left);
            ++cur_pos;

            Node* rhs = Term (text, cur_pos);

            if (rhs == nullptr) {
                std::cout << "Wrong format: expected a number or expression\n";
                delete mult;
                return nullptr;
            }

            mult->setRhs (rhs);
            m_left = (Node*) mult;
            ++cur_pos;
            cur_lex = GetLexem (text, cur_pos);
        }
    }

    return m_left;
}

Node* Term (std::vector<char>& text, unsigned& cur_pos) {
    Node* term = nullptr;
    Node* cur_lex = GetLexem (text, cur_pos);

    Node_t type = cur_lex->getType();
    if (type == END)
        return nullptr;

    if (type == NUM || type == FUNC)
        return cur_lex;

    if (type == VARNAME) {
        //TODO add in unordered map
    }

    if (IsLBrace(cur_lex)) {
        ++cur_pos;
        term = Expression (text, cur_pos);
        ++cur_pos;

        delete cur_lex;
        cur_lex = GetLexem (text, cur_pos);

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

