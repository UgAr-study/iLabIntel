#include "../include/ExpressionHeader.h"

Expr::Expr(std::vector<char> &text, unsigned &cur_pos) : Node(nullptr, EXPR) {
    top = BuildSyntaxTree (text, cur_pos);
}

int Expr::Culculate() {
    return TreeCalculator (top);
}

