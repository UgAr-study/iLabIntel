#pragma once

#include "MainHeader.h"

Node* BuildSyntaxTree (std::vector<Node *>::iterator &cur_iter, VarTable& variables);
Node* Expression      (std::vector<Node *>::iterator &cur_iter, VarTable& variables);
Node* Multiplication  (std::vector<Node *>::iterator &cur_iter, VarTable& variables);
Node* Term            (std::vector<Node *>::iterator &cur_iter, VarTable& variables);

int TreeCalculator (const Node* top, std::unordered_map<std::string, int>& values);