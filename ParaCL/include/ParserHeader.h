#pragma once
#include "MainHeader.h"

std::vector<char>
ReadFile (const char* filename);

void Parser (std::vector<Node*>::iterator begin,
             std::vector<Node*>::iterator end,
             VarValues* GlobalValues);
