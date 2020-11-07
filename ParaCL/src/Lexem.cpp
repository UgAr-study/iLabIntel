#include "../include/LexemHeader.h"

std::vector<Node*>
Lexer (std::vector<char>& text) {
    std::vector<Node*> lexems;
    int l_num = 1;
    unsigned cur_pos = 0;
    for (; cur_pos < text.size(); ++cur_pos) {
        Node* cur_lexem = GetLexem(text, cur_pos, l_num);

        if (cur_lexem == nullptr) {
            std::vector<Node*> bad_vec;
            return bad_vec;
        }

        lexems.push_back(cur_lexem);
    }
    return lexems;
}

Node* GetLexem (std::vector<char>& text, unsigned& cur_pos, int& l_num) {

    while (std::isspace(text[cur_pos])) {
        if (text[cur_pos] == '\n')
            ++l_num;
        ++cur_pos;
    }

    if (IsEnd(text[cur_pos])) {
        auto res = new End{l_num};
        return res;
    }

    if (std::isalpha(text[cur_pos])) {
        std::string name = GetName (text, cur_pos);
        if (name == "print") {
            auto res = new Func{PRINT, l_num};
            return res;
        } else if (name == "if") {
            auto res = new Branch_Operator{IF, l_num};
            return res;
        } else if (name == "while") {
            auto res = new Branch_Operator{WHILE, l_num};
            return res;
        } else {
            auto res = new VarName{name, l_num};
            return res;
        }
    }

    if (std::isdigit(text[cur_pos])) {
        int num = GetNumber (text, cur_pos);
        auto res = new Num {num, l_num};
        return res;
    }


    switch (text[cur_pos]) {
        case '+': {
            auto res = new BinOp{ADD, l_num};
            return res;
        }
        case '-': {
            auto res = new BinOp{SUB, l_num};
            return res;
        }
        case '*': {
            auto res = new BinOp{MULT, l_num};
            return res;
        }
        case '/': {
            auto res = new BinOp{DIV, l_num};
            return res;
        }
        case '=': {

            while (std::isspace(text[cur_pos + 1])) {
                if (text[cur_pos] == '\n')
                    ++l_num;
                ++cur_pos;
            }

            if (text[cur_pos + 1] == '=') {
                auto res = new BinOp{EQUAL, l_num};
                cur_pos++;
                return res;
            }

            auto res = new BinOp{ASSIGN, l_num};
            return res;
        }

        case '<': {
            while (std::isspace(text[cur_pos + 1])) {
                if (text[cur_pos] == '\n')
                    ++l_num;
                ++cur_pos;
            }

            if (text[cur_pos + 1] == '=') {
                auto res = new BinOp{LESSEQUAL, l_num};
                cur_pos++;
                return res;
            }

            auto res = new BinOp{LESS, l_num};
            return res;
        }

        case '>': {
            while (std::isspace(text[cur_pos + 1])) {
                if (text[cur_pos] == '\n')
                    ++l_num;
                ++cur_pos;
            }

            if (text[cur_pos + 1] == '=') {
                auto res = new BinOp{OVEREQUAL, l_num};
                cur_pos++;
                return res;
            }

            auto res = new BinOp{OVER, l_num};
            return res;
        }

        case '!': {
            while (std::isspace(text[cur_pos + 1])) {
                if (text[cur_pos] == '\n')
                    ++l_num;
                ++cur_pos;
            }

            if (text[cur_pos + 1] == '=') {
                auto res = new BinOp{NOTEQUAL, l_num};
                cur_pos++;
                return res;
            }

            std::cout << "Error: line " << l_num << ": after '!' expected '=' " << std::endl;
            Node* res = nullptr;
            return res;
        }
        case '(': {
            auto res = new Brace{LROUNDBRACK, l_num};
            return res;
        }
        case ')': {
            auto res = new Brace{RROUNDBRACK, l_num};
            return res;
        }
        case '{': {
            auto res = new Brace{OPENBRACE, l_num};
            return res;
        }
        case '}': {
            auto res = new Brace{CLOSEBRACE, l_num};
            return res;
        }
        case '?': {
            auto res = new Func {SCAN, l_num};
            return res;
        }
        default:
            std::cout << "Error: line " << l_num << ": Unknown symbol" << std::endl;
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