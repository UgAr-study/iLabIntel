#include "../include/MainHeader.h"
#include "../include/LexemHeader.h"
#include "../include/ExpressionHeader.h"

Node* BuildSyntaxTree (std::vector<Node *>::iterator &cur_iter, VarTable& variables) {
    Node* tree;
    tree = Expression (cur_iter, variables);

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

Node* Expression (std::vector<Node *>::iterator &cur_iter, VarTable& variables) {
    Node *e_left;
    BinOp *expression;

    e_left = Multiplication (cur_iter, variables);

    if (e_left != nullptr) {
        Node* cur_lex = *(++cur_iter);

        if (cur_lex->getType() == END) {
            //delete cur_lex;
            --cur_iter;
            return e_left;
        }

        while (IsAddSub(cur_lex)) {
            expression = static_cast<BinOp*>(cur_lex);
            expression->setLhs (e_left);
            ++cur_iter;

            Node* rhs = Multiplication (cur_iter, variables);

            if (rhs == nullptr) {
                std::cout << "Wrong format: expected a number or expression\n";
                delete expression;
                return nullptr;
            }

            expression->setRhs (rhs);
            e_left = static_cast<Node*>(expression);

            cur_lex = *(++cur_iter);
            if (!IsAddSub(cur_lex))
                --cur_iter;
        }
    }

    return e_left;
}

Node* Multiplication (std::vector<Node *>::iterator &cur_iter, VarTable& variables) {
    Node* m_left;
    BinOp* mult;

    m_left = Term (cur_iter, variables);

    if (m_left != nullptr) {
        Node* cur_lex = *(++cur_iter);

        if (cur_lex->getType() == END) {
            //delete cur_lex;
            --cur_iter;
            return m_left;
        }

        while (IsMulDiv(cur_lex)) {
            mult = static_cast<BinOp*>(cur_lex);
            mult->setLhs(m_left);
            ++cur_iter;

            Node* rhs = Term (cur_iter, variables);

            if (rhs == nullptr) {
                std::cout << "Wrong format: expected a number or expression\n";
                delete mult;
                return nullptr;
            }

            mult->setRhs (rhs);
            m_left = static_cast<Node*>(mult);

            cur_lex = *(++cur_iter);
            if (!IsMulDiv(cur_lex))
                --cur_iter;
        }
    }

    return m_left;
}

Node* Term (std::vector<Node *>::iterator &cur_iter, VarTable& variables) {
    Node* term = nullptr;
    Node* cur_lex = *cur_iter;

    Node_t type = cur_lex->getType();
    if (type == END)
        return nullptr;

    if (type == NUM || type == FUNC)
        return cur_lex;

    if (type == VARNAME) {
        VarName var = *(static_cast<VarName*>(cur_lex));
        if (variables.find(var.getName()) == variables.end()) {
            std::cout << "Error: unknown variable [" << var.getName() << "]" << std::endl;
            return nullptr;
        }
        return static_cast<Node*>(variables[var.getName()]);
    }

    if (IsLBrace(cur_lex)) {
        term = Expression (++cur_iter, variables);

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

int TreeCalculator (const Node* top, std::unordered_map<std::string, int>& values) {
    if (top == nullptr) {
        std::cout << "Nothing to calculate\n";
        return 0;
    }

    int result = 0, left = 0, right = 0;

    if (top->getType() == NUM) {
        Num num = *((Num*) top);
        return num.getNum();
    }

    if (top->getType() == BINOP) {
        BinOp op = *(static_cast<const BinOp*>(top));
        if (op.getOperation() == ASSIGN) {
            VarName var = *(static_cast<const VarName*>(op.getLhs()));

            if (values.find(var.getName()) != values.end()) {
                return values[var.getName()];
            }



            if ((op.getRhs())->getType() == EXPR) {
                const Expr expr = *(static_cast<const Expr*>(op.getRhs()));
                if (expr.getTopType() == FUNC) {
                    Func func = *(static_cast<const Func*>(op.getRhs()));
                    if (func.getFunction() == SCAN) {
                        std::cout << "Please, input this variable: " << var.getName() << " = ";
                        std::cin >> result;
                        values[var.getName()] = result;
                        return result;
                    }
                }
                result = TreeCalculator(expr.getTop(), values);
                values[var.getName()] = result;
                return result;
            }
            std::cout << "Error: unknown operation for ASSIGN\n";
        }

        left = TreeCalculator (op.getLhs(), values);
        right = TreeCalculator (op.getRhs(), values);
        switch (op.getOperation()) {

            case ADD:
                result = left + right;
                std::cout << left << " + " << right << "   ";
                break;
            case SUB:
                result = left - right;
                std::cout << left << " - " << right << "   ";
                break;
            case MULT:
                result = left * right;
                std::cout << left << " * " << right << "   ";
                break;
            case DIV:
                result = left / right;
                std::cout << left << " / " << right << "   ";
                break;
            case ASSIGN:
                std::cout << "ASSIGN: Oh shit! Here we go again..." << std::endl;
                break;
        }
        return result;
    }

    std::cout << "Something went wrong while calculating\n";
    return 0;
}

void PrintTree (const Node* top) {
    if (top == nullptr) {
        std::cout << "Top is nullptr!\n";
        return;
    }

    if (top->getType() == BINOP) {
        BinOp op = *(static_cast<const BinOp*>(top));
        PrintLexem(top);
        std::cout << std::endl;
        PrintLexem(op.getLhs());
        std::cout << std::endl;
        PrintLexem(op.getRhs());
        std::cout << std::endl << std::endl;

        PrintTree(op.getLhs());
        PrintTree(op.getRhs());
    }
}

