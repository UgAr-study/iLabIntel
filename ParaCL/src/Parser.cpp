#include "../include/ParserHeader.h"

std::vector<char>
ReadFile (const char* filename) {

    std::vector<char> text;

    FILE* file = std::fopen(filename, "r");
    if (file == nullptr) {
        std::cout << "Some problems with file" << std::endl;
        return text;
    }

    struct stat buf{};
    fstat (fileno(file), &buf);
    unsigned size = buf.st_size;


    text.reserve(size);
    char c;
    while ((c = std::fgetc(file)) != EOF) {
        text.push_back(c);
    }

    std::fclose(file);
    return text;
}

void Parser (std::vector<Node*>::iterator begin,
             std::vector<Node*>::iterator end,
             VarValues* GlobalValues) {


    VarValues* values;
    if (GlobalValues != nullptr)
        values = GlobalValues;
    else
        values = new std::unordered_map<std::string, Num*>;

    auto lexem = begin;
    for (; lexem != end; ++lexem) {
        Node_t type = (*lexem)->getType();

        if (type == BRANCHOPERATOR) {
            Branch_Operator branch_operator = *(static_cast<Branch_Operator*>(*lexem));

            if (branch_operator.getOperatorType() == IF) {
                auto if_operator = new Branch_Operator{IF, (++lexem), end, *values};

                if (!if_operator->isValid()) {
                    std::cout << "Error: line " << branch_operator.getLineNumber() << ": 'IF' operator is not valid" << std::endl;
                    return;
                }

                if (if_operator->isConditionTrue(*values)) {
                    Parser(if_operator->getBeginScope(),if_operator->getEndScope(), values);
                }
            }

            if (branch_operator.getOperatorType() == WHILE) {
                auto while_operator = new Branch_Operator{WHILE, (++lexem), end, *values};

                if (!while_operator->isValid()) {
                    std::cout << "Error: line " << branch_operator.getLineNumber() << ": 'WHILE' operator is not valid" << std::endl;
                    return;
                }

                while (while_operator->isConditionTrue(*values)) {
                    Parser(while_operator->getBeginScope(), while_operator->getEndScope(), values);
                }
            }
            continue;
        }

        if (type == FUNC) {
            auto func = static_cast<Func*>(*lexem);

            if (func->getFunction() == PRINT) {
                ++lexem;

                func->setExpr(lexem, *values);
                std::cout << func->CulcExpression(*values) << std::endl;
            }
            else if (func->getFunction() == SCAN) {
                std::cout << "Error: line " << func->getLineNumber() << ": not expected SCAN here\n";
                return;
            }
            continue;
        }

        if (type == VARNAME) {
            auto var = static_cast<VarName*>(*lexem);

            if ((*(++lexem))->getType() == BINOP) {
                auto bop = static_cast<BinOp*>(*lexem);

                if (bop->getOperation() == ASSIGN) {
                    Expr *rhs = new Expr{++lexem, *values, (*lexem)->getLineNumber()};
                    bop->setLhs(var);
                    bop->setRhs(rhs);
                    (*values)[var->getName()] = new Num(rhs->Culculate(*values), var->getLineNumber());
                }
                else {
                    std::cout << "Error: line " << bop->getLineNumber() << ": don't know what to do with this variable: " << var->getName() << std::endl;
                    return;
                }
            }
            else {
                std::cout << "Error: line " << var->getLineNumber() << ": don't know what to do with this variable: " << var->getName() << std::endl;
                return;
            }

            continue;
        }

        if (type == END)
            continue;
        else {
            std::cout << "Error: line " << (*lexem)->getLineNumber() << ": expected var or function\n";
            return;
        }
    }

    if (GlobalValues == nullptr)
        delete values;
}

