#pragma once
#include "MainHeader.h"

std::vector<char>
ReadFile (const char* filename);
void Parser (std::vector<Node*>& lexems, VarValues * GlobalValues);
