#include "../include/ExpressionHeader.h"
#include "../include/MainHeader.h"


/*
Expr::Expr(std::vector<char> &text, unsigned &cur_pos) : Node(nullptr, EXPR) {
    top = BuildSyntaxTree (text, cur_pos);
}
*/

int Expr::Culculate() {
    return TreeCalculator (top);
}

Expr::Expr(std::vector<Node *>::iterator &cur_iter) : Node(nullptr, EXPR){
    top = BuildSyntaxTree(cur_iter);
}

