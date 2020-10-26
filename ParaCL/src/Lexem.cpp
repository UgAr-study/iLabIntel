#include "../include/LexemHeader.h"

std::vector<Node*>
Lexer (std::vector<char>& text) {
    std::vector<Node*> lexems;
    unsigned cur_pos = 0;
    for (; cur_pos < text.size(); ++cur_pos) {
        lexems.push_back(GetLexem(text, cur_pos));
        ++cur_pos;
    }
    return lexems;
}

Node* GetLexem (std::vector<char>& text, unsigned& cur_pos) {
    int flag = 0;

    while (std::isspace(text[cur_pos++]));

    if (IsEnd(text[cur_pos])) {
        auto res = new Node{nullptr, END};
        return res;
    }

    if (std::isalpha(text[cur_pos])) {
        std::string name = GetName (text, cur_pos);
        if (name == "print") {
            auto res = new Func{PRINT};
            return res;
        } else if (name == "if") {
            //TODO
        } else if (name == "while") {
            //TODO
        } else {
            auto res = new VarName{name};
            return res;
        }
    }

    //while (std::isspace(text[cur_pos++]));

    if (std::isdigit(text[cur_pos])) {
        int num = GetNumber (text, cur_pos);
        auto res = new Num {num};
        return res;
    }


    switch (text[cur_pos++]) {
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
            auto res = new BinOp{ASSIGN};
            return res;
        }
        case '(': {
            auto res = new Brace{LBRACE};
            return res;
        }
        case ')': {
            auto res = new Brace{RBRACE};
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
    if (c == '\0')
        return true;
    return false;
}

bool IsAddSub (Node* lex) {
    if (lex == nullptr)
        return false;
    if (lex->getType() == BINOP) {
        BinOp op = *((BinOp*) lex);
        BinOp_t type = op.getOperation();
        if (type == SUB || type == ADD)
            return true;
    }
    //TODO do I need to decrement cur_pos?
    return false;
}

bool IsMulDiv (Node* lex) {
    if (lex == nullptr)
        return false;
    if (lex->getType() == BINOP) {
        BinOp op = *((BinOp*) lex);
        BinOp_t type = op.getOperation();
        if (type == MULT || type == DIV)
            return true;
    }

    //TODO do I need to decrement cur_pos?
    return false;
}

bool IsLBrace (Node* lex) {
    if (lex == nullptr)
        return false;
    if (lex->getType() == BRACE) {
        Brace brace = *((Brace*) lex);
        if (brace.getBraceType() == LBRACE)
            return true;
    }
    return false;
}

bool IsRBrace (Node* lex) {
    if (lex == nullptr)
        return false;
    if (lex->getType() == BRACE) {
        Brace brace = *((Brace*) lex);
        if (brace.getBraceType() == RBRACE)
            return true;
    }
    return false;
}

std::string
GetName (std::vector<char>& text, unsigned& cur_pos) {
    std::vector<char> build_name;

    while (std::isalpha(text[cur_pos])) {
        build_name.push_back(text[cur_pos]);
        ++cur_pos;
    }

    std::string name {build_name.begin(), build_name.end()};
    return name;
}

int GetNumber (std::vector<char>& text, unsigned& cur_pos) {
    std::vector<char> strnum;
    while (std::isdigit(text[cur_pos])) {
        strnum.push_back(text[cur_pos]);
        ++cur_pos;
    }
    int res = std::stoi(std::string {strnum.begin(), strnum.end()});
    return res;
}

void PrintLexem (const Node* lex) {
    switch (lex->getType()) {

        case BINOP: {
            BinOp binop = *((BinOp*) lex);
            switch (binop.getOperation()) {

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
            }

            break;
        }
        case EXPR:
            std::cout << "Expression";
            break;
        case FUNC: {
            Func func = *((Func*) lex);

            switch (func.getFunction()) {
                case SCAN:
                    std::cout << "Func: SCAN";
                    break;
                case PRINT:
                    std::cout << "Func: PRINT";
                    break;
            }

            break;
        }
        case OPERATOR:
            std::cout << "Operator: don't know yet :(";
            break;
        case VARNAME: {
            VarName var = *((VarName*) lex);
            std::cout << "VAR: " << var.getName();
            break;
        }
        case BRACE: {
            Brace brace = *((Brace*) lex);

            switch (brace.getBraceType()) {
                case LBRACE:
                    std::cout << "Brace: LBRACE";
                    break;
                case RBRACE:
                    std::cout << "Brace: RBRACE";
                    break;
            }

            break;
        }
        case NUM: {
            Num num = *((Num*) lex);
            std::cout << "Num: " << num.getNum();
            break;
        }
        case END:
            std::cout << "End";
            break;
    }
}