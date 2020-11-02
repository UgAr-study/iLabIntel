#pragma once
#include "MainHeader.h"

std::vector<Node*>
Lexer (std::vector<char>& text);

Node* GetLexem (std::vector<char>& text, unsigned& cur_pos);

bool IsCorrectSymbolForVarName (char c);
std::string
GetName (std::vector<char>& text, unsigned& cur_pos);

int GetNumber (std::vector<char>& text, unsigned& cur_pos);
void Indents(int n);
void PrintLexem (const Node* lex, int n_tabs);

bool IsAddSub (const Node* lex);
bool IsMulDiv (const Node* lex);
bool IsLBrace (const Node* lex);
bool IsRBrace (const Node* lex);
bool IsEnd (char c);
