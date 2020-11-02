#include "../include/LexemHeader.h"

std::vector<Node*>
Lexer (std::vector<char>& text) {
    std::vector<Node*> lexems;
    unsigned cur_pos = 0;
    for (; cur_pos < text.size(); ++cur_pos) {
        lexems.push_back(GetLexem(text, cur_pos));
    }
    return lexems;
}

Node* GetLexem (std::vector<char>& text, unsigned& cur_pos) {

    while (std::isspace(text[cur_pos]))
        ++cur_pos;

    if (IsEnd(text[cur_pos])) {
        auto res = new End{};
        return res;
    }

    if (std::isalpha(text[cur_pos])) {
        std::string name = GetName (text, cur_pos);
        if (name == "print") {
            auto res = new Func{PRINT};
            return res;
        } else if (name == "if") {
            auto res = new Branch_Operator{IF};
            return res;
        } else if (name == "while") {
            auto res = new Branch_Operator{WHILE};
            return res;
        } else {
            auto res = new VarName{name};
            return res;
        }
    }

    if (std::isdigit(text[cur_pos])) {
        int num = GetNumber (text, cur_pos);
        auto res = new Num {num};
        return res;
    }


    switch (text[cur_pos]) {
        case '+': {
            auto res = new BinOp{ADD};
            return res;
        }
        case '-': {
            auto res = new BinOp{SUB};
            return res;
        }
        case '*': {
            auto res = new BinOp{MULT};
            return res;
        }
        case '/': {
            auto res = new BinOp{DIV};
            return res;
        }
        case '=': {

            while (std::isspace(text[cur_pos + 1]))
                ++cur_pos;

            if (text[cur_pos + 1] == '=') {
                auto res = new BinOp{EQUAL};
                cur_pos++;
                return res;
            }

            auto res = new BinOp{ASSIGN};
            return res;
        }

        case '<': {
            while (std::isspace(text[cur_pos + 1]))
                ++cur_pos;

            if (text[cur_pos + 1] == '=') {
                auto res = new BinOp{LESSEQUAL};
                cur_pos++;
                return res;
            }

            auto res = new BinOp{LESS};
            return res;
        }

        case '>': {
            while (std::isspace(text[cur_pos + 1]))
                ++cur_pos;

            if (text[cur_pos + 1] == '=') {
                auto res = new BinOp{OVEREQUAL};
                cur_pos++;
                return res;
            }

            auto res = new BinOp{OVER};
            return res;
        }

        case '!': {
            while (std::isspace(text[cur_pos + 1]))
                ++cur_pos;

            if (text[cur_pos + 1] == '=') {
                auto res = new BinOp{NOTEQUAL};
                cur_pos++;
                return res;
            }

            Node* res = nullptr;
            return res;
        }
        case '(': {
            auto res = new Brace{LROUNDBRACK};
            return res;
        }
        case ')': {
            auto res = new Brace{RROUNDBRACK};
            return res;
        }
        case '{': {
            auto res = new Brace{OPENBRACE};
            return res;
        }
        case '}': {
            auto res = new Brace{CLOSEBRACE};
            return res;
        }
        case '?': {
            auto res = new Func {SCAN};
            return res;
        }
    }

    return nullptr;
}

bool IsEnd (char c) {
    if (c == '\0' || c == ';')
        return true;
    return false;
}

bool IsAddSub (const Node* lex) {
    if (lex == nullptr)
        return false;
    if (lex->getType() == BINOP) {
        auto op = static_cast<const BinOp*>(lex);
        BinOp_t type = op->getOperation();
        if (type == SUB || type == ADD)
            return true;
    }

    return false;
}

bool IsMulDiv (const Node* lex) {
    if (lex == nullptr)
        return false;
    if (lex->getType() == BINOP) {
        auto op =  static_cast<const BinOp*>(lex);
        BinOp_t type = op->getOperation();
        if (type == MULT || type == DIV)
            return true;

    }

    return false;
}

bool IsLBrace (const Node* lex) {
    if (lex == nullptr)
        return false;
    if (lex->getType() == BRACE) {
        auto brace = static_cast<const Brace*>(lex);
        if (brace->getBraceType() == LROUNDBRACK)
            return true;
    }
    return false;
}

bool IsRBrace (const Node* lex) {
    if (lex == nullptr)
        return false;
    if (lex->getType() == BRACE) {
        auto brace = static_cast<const Brace*>(lex);
        if (brace->getBraceType() == RROUNDBRACK)
            return true;
    }
    return false;
}

bool IsCorrectSymbolForVarName (char c) {
    if (std::isalpha(c) || std::isdigit(c) || c == '_' || c == '-')
        return true;
    return false;
}


std::string
GetName (std::vector<char>& text, unsigned& cur_pos) {
    std::vector<char> build_name;

    while (IsCorrectSymbolForVarName(text[cur_pos])) {
        build_name.push_back(text[cur_pos]);
        ++cur_pos;
    }
    --cur_pos;
    std::string name {build_name.begin(), build_name.end()};
    return name;
}

int GetNumber (std::vector<char>& text, unsigned& cur_pos) {
    std::vector<char> strnum;
    while (std::isdigit(text[cur_pos])) {
        strnum.push_back(text[cur_pos]);
        ++cur_pos;
    }
    --cur_pos;
    int res = std::stoi(std::string {strnum.begin(), strnum.end()});
    return res;
}

void Indents (int n) {
    while (n > 0) {
        std::cout << "  ";
        n--;
    }
}

void PrintLexem (const Node* lex, int n_tabs) {
    Indents(n_tabs);

    switch (lex->getType()) {

        case BINOP: {
            auto bin_op = static_cast<const BinOp*>(lex);
            switch (bin_op->getOperation()) {

                case ADD:
                    std::cout << "BinOp: ADD";
                    break;
                case SUB:
                    std::cout << "BinOp: SUB";
                    break;
                case MULT:
                    std::cout << "BinOp: MULT";
                    break;
                case DIV:
                    std::cout << "BinOp: DIV";
                    break;
                case ASSIGN:
                    std::cout << "BinOp: ASSIGN";
                    break;
                case EQUAL:
                    std::cout << "BinOp: EQUAL";
                    break;
                case NOTEQUAL:
                    std::cout << "BinOp: NOTEQUAL";
                    break;
                case LESS:
                    std::cout << "BinOp: LESS";
                    break;
                case OVER:
                    std::cout << "BinOp: OVER";
                    break;
                case LESSEQUAL:
                    std::cout << "BinOp: LESSEQUAL";
                    break;
                case OVEREQUAL:
                    std::cout << "BinOp: OVEREQUAL";
                    break;
            }

            break;
        }
        case EXPR:
            std::cout << "Expression";
            break;
        case FUNC: {
            auto func = static_cast<const Func*>(lex);

            switch (func->getFunction()) {
                case SCAN:
                    std::cout << "Func: SCAN";
                    break;
                case PRINT:
                    std::cout << "Func: PRINT";
                    break;
            }

            break;
        }
        case BRANCHOPERATOR: {
            auto branch = static_cast<const Branch_Operator*>(lex);
            if (branch->getOperatorType() == IF)
                std::cout << "Branch_Operator: IF";
            if (branch->getOperatorType() == WHILE)
                std::cout << "Branch_Operator: IF";
            else
                std::cout << "Branch_Operator: don't know yet :(";
            break;
        }
        case VARNAME: {
            auto var = static_cast<const VarName*>(lex);
            std::cout << "VAR: " << var->getName();
            break;
        }
        case BRACE: {
            auto brace = static_cast<const Brace*>(lex);

            switch (brace->getBraceType()) {
                case LROUNDBRACK:
                    std::cout << "Brace: LROUNDBRACK";
                    break;
                case RROUNDBRACK:
                    std::cout << "Brace: RROUNDBRACK";
                    break;
                case OPENBRACE:
                    std::cout << "Brace: OPENBRACE";
                    break;
                case CLOSEBRACE:
                    std::cout << "Brace: CLOSEBRACE";
                    break;
            }
            break;
        }
        case NUM: {
            auto num = static_cast<const Num*>(lex);
            std::cout << "Num: " << num->getNum();
            break;
        }
        case END:
            std::cout << "End";
            break;
    }

    std::cout << std::endl;
}