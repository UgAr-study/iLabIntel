#pragma once

#include "../GeometryHeader.h"
#include "../AlgorithmHeader.h"
#include <ctime>
#include <algorithm>

enum {MAXCOORD = 50, NTRS = 300};

std::vector<int>
IntGenerator (int n, int min, int max);

std::vector<Geom::Triangle>
GetRandomTriangles (int tr_num);

std::unordered_map<unsigned, std::vector<unsigned>>
GetAnswer (std::vector<Geom::Triangle> &trs);

void DumpAnswer (std::unordered_map<unsigned, std::vector<unsigned>>& ans_table);
void DumpIntersectionsWithTriangle (Geom::Triangle tr, std::vector<Geom::Triangle> &trs);