#pragma once
#include "MainHeader.h"

std::vector<Node*>
Lexer (std::vector<char>& text);

Node* GetLexem (std::vector<char>& text, unsigned& cur_pos);

std::string
GetName (std::vector<char>& text, unsigned& cur_pos);

int GetNumber (std::vector<char>& text, unsigned& cur_pos);
void PrintLexem (Node lex);
bool IsAddSub (Node* lex);

bool IsMulDiv (Node* lex);
bool IsLBrace (Node* lex);
bool IsRBrace (Node* lex);
bool IsEnd (char c);