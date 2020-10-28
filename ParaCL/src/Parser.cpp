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

void Parser (std::vector<Node*>& lexems) {

    VarTable variables;
    std::unordered_map<std::string, int> values;
    for (auto lexem = lexems.begin(); lexem != lexems.end(); ++lexem) {
        Node_t type = (*lexem)->getType();
        //if (type == END)
        //    break;

        if (type == FUNC) {
            Func func = *(static_cast<Func*>(*lexem));
            if (func.getFunction() == PRINT) {
                ++lexem;
                //Expr expr{lexem, variables};
                //func->expression = expr;
                func.setExpr(lexem, variables);
                func.getExpression().Dump();
                std::cout << func.getExpression().Culculate(values) << std::endl;
            } else if (func.getFunction() == SCAN) {
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
                    bop->setLhs(var);
                    Expr* rhs = new Expr {++lexem, variables};
                    bop->setRhs(static_cast<Node*>(rhs));
                    variables[var->getName()] = bop;
                } else {
                    std::cout << "Error: don't know what to do with this variable: " << var->getName() << std::endl;
                    return;
                }
            } else {
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

