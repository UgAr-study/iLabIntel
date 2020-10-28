#include "../include/ExpressionHeader.h"
#include "../include/MainHeader.h"


/*
Expr::Expr(std::vector<char> &text, unsigned &cur_pos) : Node(nullptr, EXPR) {
    top = BuildSyntaxTree (text, cur_pos);
}
*/

int Expr::Culculate(std::unordered_map<std::string, int>& values) const {
    return TreeCalculator (top, values);
}

Expr::Expr(std::vector<Node *>::iterator &cur_iter, VarTable& variables) : Node(nullptr, EXPR){
    top = BuildSyntaxTree(cur_iter, variables);
}

void Expr::Dump() const {
    PrintTree(top);
}

