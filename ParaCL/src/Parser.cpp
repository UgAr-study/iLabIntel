#include "../include/ParserHeader.h"
#include "../include/LexemHeader.h"

void Parser (std::vector<Node*> lexems) {

    VarTable variables;
    for (auto lexem = lexems.begin(); lexem != lexems.end(); ++lexem) {
        Node_t type = (*lexem)->getType();
        if (type == END)
            break;

        if (type == FUNC) {
            Func func = *(static_cast<Func*>(*lexem));
            if (func.getFunction() == PRINT) {
                func.
            }
        }
    }

    for (; cur_pos < text.size(); ++cur_pos) {
        Node* cur_lex = GetLexem(text, cur_pos);
        Node_t type = cur_lex->getType();
        if (type == END)
            break;

        if (type != VARNAME && type != FUNC) {
            std::cout << "\nWrong format: expected variable or function in the beginning of the string\n";
            return;
        }

        if (type == FUNC) {
            Func func = *((Func*) cur_lex);
            if (func.getFunction() == PRINT) {
                //TODO Let expression in Func will contain all info
                //TODO and the method Calc will calculate it
            }
        }
    }
}

