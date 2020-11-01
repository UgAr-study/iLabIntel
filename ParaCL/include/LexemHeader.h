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
bool IsAddSub (Node* lex);

bool IsMulDiv (Node* lex);
bool IsLBrace (Node* lex);
bool IsRBrace (Node* lex);
bool IsEnd (char c);
