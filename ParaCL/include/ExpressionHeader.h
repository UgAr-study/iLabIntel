#pragma once

#include "MainHeader.h"

Node* BuildSyntaxTree (std::vector<char>& text, unsigned& cur_pos);
Node* Expression      (std::vector<char>& text, unsigned& cur_pos);
Node* Multiplication  (std::vector<char>& text, unsigned& cur_pos);
Node* Term            (std::vector<char>& text, unsigned& cur_pos);

int TreeCalculator (Node* top);