#include "../include/MainHeader.h"
#include "../include/LexemHeader.h"
#include "../include/ExpressionHeader.h"

Node* BuildSyntaxTree (std::vector<Node *>::iterator &cur_iter, VarValues& values) {
    Node* tree;
    tree = Expression (cur_iter, values);

    if (tree == nullptr) {
        printf ("Nothing to be calculated\n");
        exit (0);
    }

    return tree;
}

Node* Expression (std::vector<Node *>::iterator &cur_iter, VarValues& values) {
    Node *e_left;
    BinOp *expression;

    e_left = Multiplication (cur_iter, values);

    if (e_left != nullptr) {
        Node* cur_lex = *(++cur_iter);

        if (cur_lex->getType() == END) {
            --cur_iter;
            return e_left;
        }

        if (IsAddSub(cur_lex)) {

            while (IsAddSub(cur_lex)) {
                expression = static_cast<BinOp *>(cur_lex);
                expression->setLhs(e_left);
                ++cur_iter;

                Node *rhs = Multiplication(cur_iter, values);

                if (rhs == nullptr) {
                    std::cout << "Wrong format: line " << (*cur_iter)->getLineNumber() << ": expected a number or expression\n";
                    delete expression;
                    return nullptr;
                }

                expression->setRhs(rhs);
                e_left = static_cast<Node *>(expression);

                cur_lex = *(++cur_iter);
            }
        }
        --cur_iter;
    }

    return e_left;
}

Node* Multiplication (std::vector<Node *>::iterator &cur_iter, VarValues& values) {
    Node* m_left;
    BinOp* mult;

    m_left = Term (cur_iter, values);

    if (m_left != nullptr) {
        Node* cur_lex = *(++cur_iter);

        if (cur_lex->getType() == END) {
            --cur_iter;
            return m_left;
        }

        if (IsMulDiv(cur_lex)) {

            while (IsMulDiv(cur_lex)) {
                mult = static_cast<BinOp *>(cur_lex);
                mult->setLhs(m_left);
                ++cur_iter;

                Node *rhs = Term(cur_iter, values);

                if (rhs == nullptr) {
                    std::cout << "Wrong format: line " << (*cur_iter)->getLineNumber() <<": expected a number or expression\n";
                    delete mult;
                    return nullptr;
                }

                mult->setRhs(rhs);
                m_left = static_cast<Node *>(mult);

                cur_lex = *(++cur_iter);
            }
        }

        --cur_iter;
    }

    return m_left;
}

Node* Term (std::vector<Node *>::iterator &cur_iter, VarValues& values) {
    Node* term;
    Node* cur_lex = *cur_iter;

    Node_t type = cur_lex->getType();
    if (type == END)
        return nullptr;

    if (type == NUM || type == FUNC)
        return cur_lex;

    if (type == BINOP) {
        auto binop = static_cast<BinOp*>(cur_lex);

        if (binop->getOperation() == SUB) {
            Node* rhs = Expression(++cur_iter, values);

            if (rhs == nullptr) {
                std::cout << "Wrong format: line " << binop->getLineNumber() << ": expected a number or expression\n";
                return nullptr;
            }

            binop->setRhs(rhs);
            return cur_lex;
        } else {
            std::cout << "Error: line " << binop->getLineNumber() << ": unknown operation\n";
            return nullptr;
        }
    }

    if (type == VARNAME) {
        auto var = static_cast<VarName*>(cur_lex);
        if (values.find(var->getName()) == values.end()) {
            std::cout << "Error: line " << var->getLineNumber() << ": unknown variable [" << var->getName() << "]" << std::endl;
            return nullptr;
        }
        return cur_lex;
    }

    if (IsLBrace(cur_lex)) {
        term = Expression (++cur_iter, values);

        cur_lex = *(++cur_iter);

        if (!IsRBrace(cur_lex)) {
            std::cout << "Wrong format: line " << cur_lex->getLineNumber() << ": expected ')'\n";
            return nullptr;
        }

        return term;
    } else {
        std::cout << "Wrong format: line " << cur_lex->getLineNumber() << ": expected variable or brace\n";
        return nullptr;
    }
}

int TreeCalculator (const Node* top, VarValues & values) {
    if (top == nullptr) {
        std::cout << "Nothing to calculate\n";
        return 0;
    }

    int result = 0, left = 0, right = 0;

    if (top->getType() == NUM) {
        auto num = static_cast<const Num*>(top);
        return num->getNum();
    }

    if (top->getType() == VARNAME) {
        auto var = static_cast<const VarName*>(top);

        if (values.find(var->getName()) == values.end()) {
            std::cout << "Error: line " << var->getLineNumber() << ": unknown variable: " << var->getName() << std::endl;
            return 0;
        }

        return values[var->getName()]->getNum();
    }

    if (top->getType() == FUNC) {
        auto func = static_cast<const Func*>(top);

        if (func->getFunction() == SCAN) {
            std::cin >> result;
            return result;
        }
        else {
            std::cout << "Error: line " << func->getLineNumber() << ": unknow function for variable" << std::endl;
            return 0;
        }
    }


    if (top->getType() == BINOP) {
        auto op = static_cast<const BinOp*>(top);

        if (op->getOperation() == ASSIGN) {
            auto var = static_cast<const VarName*>(op->getLhs());

            if (values.find(var->getName()) != values.end()) {
                return values[var->getName()]->getNum();
            }

            if ((op->getRhs())->getType() == EXPR) {
                auto expr = static_cast<const Expr*>(op->getRhs());

                if (expr->getTopType() == FUNC) {
                    auto func = static_cast<const Func*>(op->getRhs());

                    if (func->getFunction() == SCAN) {
                        std::cin >> result;
                        values[var->getName()] = new Num{result, var->getLineNumber()};
                        return result;
                    }
                }
                result = TreeCalculator(expr->getTop(), values);
                values[var->getName()] = new Num{result, var->getLineNumber()};
                return result;
            }
            std::cout << "Error: line " << var->getLineNumber() << ": unknown operation for ASSIGN" << std::endl;
        }

        if (op->getLhs() == nullptr && op->getOperation() == SUB) {
            return -TreeCalculator(op->getRhs(), values);
        }

        left = TreeCalculator (op->getLhs(), values);
        right = TreeCalculator (op->getRhs(), values);
        switch (op->getOperation()) {

            case ADD:
                result = left + right;
                break;
            case SUB:
                result = left - right;
                break;
            case MULT:
                result = left * right;
                break;
            case DIV:
                result = left / right;
                break;
            case ASSIGN:
                std::cout << "Error: line " << op->getLineNumber() << ": '=' not expected here" << std::endl;
                break;
        }
        return result;
    }

    std::cout << "Error: line " << top->getLineNumber() << ": something went wrong while calculating\n";
    return 0;
}

void PrintTree (const Node* top, int n_tabs) {

    Indents(n_tabs);

    if (top == nullptr) {
        std::cout << "Top is nullptr!\n";
        return;
    }

    if (top->getType() == BINOP) {
        auto op = static_cast<const BinOp*>(top);
        PrintLexem(top, n_tabs);

        PrintLexem(op->getLhs(), n_tabs);
        PrintLexem(op->getRhs(), n_tabs);
        std::cout << std::endl << std::endl;

        Indents(n_tabs);
        std::cout << "Left:" << std::endl;
        PrintTree(op->getLhs(), n_tabs + 1);

        Indents(n_tabs);
        std::cout << "Right:" << std::endl;
        PrintTree(op->getRhs(), n_tabs + 1);

        return;
    }

    if (top->getType() == EXPR) {
        auto expr = static_cast<const Expr*>(top);
        PrintLexem(top, n_tabs);
        PrintTree(expr->getTop(), n_tabs + 1);
        return;
    }

    PrintLexem(top, n_tabs);
    std::cout << std::endl;
}

