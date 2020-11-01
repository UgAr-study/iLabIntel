#include "../include/ParserHeader.h"
#include "../include/ExpressionHeader.h"

std::vector<char>
ReadFile (const char* filename) {
    FILE* file = std::fopen(filename, "r");
    struct stat buf{};
    fstat (fileno(file), &buf);
    unsigned size = buf.st_size;
    std::vector<char> text;
    text.reserve(size);
    char c;
    while ((c = std::fgetc(file)) != EOF) {
        text.push_back(c);
    }
    std::fclose(file);
    return text;
}

void Parser (std::vector<Node*>& lexems, VarValues* GlobalValues) {


    VarValues* values;
    if (GlobalValues != nullptr)
        values = GlobalValues;
    else
        values = new std::unordered_map<std::string, Num*>;

    for (auto lexem = lexems.begin(); lexem != lexems.end(); ++lexem) {
        Node_t type = (*lexem)->getType();

        if (type == BRANCHOPERATOR) {
            Branch_Operator branch_operator = *(static_cast<Branch_Operator*>(*lexem));

            if (branch_operator.getOperatorType() == IF) {
                Branch_Operator if_operator{IF, (++lexem), *values};

                if (!if_operator.isValid()) {
                    std::cout << "Error: if_operator is not valid" << std::endl;
                    return;
                }

                if (if_operator.isConditionTrue(*values)) {
                    std::vector<Node*> scope_code = if_operator.getScope();
                    Parser(scope_code, values);
                }
            }

            if (branch_operator.getOperatorType() == WHILE) {
                Branch_Operator while_operator{WHILE, (++lexem), *values};

                if (!while_operator.isValid()) {
                    std::cout << "Error: if_operator is not valid" << std::endl;
                    return;
                }

                while (while_operator.isConditionTrue(*values)) {
                    std::vector<Node*> scope_code = while_operator.getScope();
                    Parser(scope_code, values);
                }
            }
            continue;
        }

        if (type == FUNC) {
            Func func = *(static_cast<Func*>(*lexem));

            if (func.getFunction() == PRINT) {
                ++lexem;

                func.setExpr(lexem, *values);
                std::cout << func.getExpression().Culculate(*values) << std::endl;
            }
            else if (func.getFunction() == SCAN) {
                std::cout << "Error: not expected SCAN here\n";
                return;
            }
            continue;
        }

        if (type == VARNAME) {
            auto var = static_cast<VarName*>(*lexem);

            if ((*(++lexem))->getType() == BINOP) {
                auto bop = static_cast<BinOp*>(*lexem);

                if (bop->getOperation() == ASSIGN) {
                    Expr rhs{++lexem, *values};
                    (*values)[var->getName()] = new Num(rhs.Culculate(*values));
                }
                else {
                    std::cout << "Error: don't know what to do with this variable: " << var->getName() << std::endl;
                    return;
                }
            }
            else {
                std::cout << "Error: don't know what to do with this variable: " << var->getName() << std::endl;
                return;
            }

            continue;
        }

        if (type == END)
            continue;
        else {
            std::cout << "Error: expected var or function\n";
            return;
        }
    }
}

