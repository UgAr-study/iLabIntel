#pragma once

#include "MainHeader.h"

Node* BuildSyntaxTree (std::vector<Node *>::iterator &cur_iter, VarValues &values);
Node* Expression      (std::vector<Node *>::iterator &cur_iter, VarValues &values);
Node* Multiplication  (std::vector<Node *>::iterator &cur_iter, VarValues &values);
Node* Term            (std::vector<Node *>::iterator &cur_iter, VarValues &values);

int TreeCalculator (const Node* top, VarValues & values);
void PrintTree (const Node* top, int n_tabs);
void RecursiveDelete(Node* top);