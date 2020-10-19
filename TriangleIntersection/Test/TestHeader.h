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

std::vector<unsigned>
GetAnswer (std::vector<Geom::Triangle> &trs);