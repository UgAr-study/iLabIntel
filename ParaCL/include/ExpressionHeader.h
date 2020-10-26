#pragma once

#include "MainHeader.h"

Node* BuildSyntaxTree (std::vector<Node *>::iterator &cur_iter);
Node* Expression      (std::vector<Node *>::iterator &cur_iter);
Node* Multiplication  (std::vector<Node *>::iterator &cur_iter);
Node* Term            (std::vector<Node *>::iterator &cur_iter);

int TreeCalculator (Node* top);