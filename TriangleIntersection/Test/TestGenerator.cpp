#include "TestHeader.h"

std::vector<int>
IntGenerator (int n, int min, int max) {
    std::vector<int> res;
    std::mt19937 generator (time(nullptr));
    std::uniform_int_distribution<int> distribution (min, max);

    res.reserve(n);
    for (int i = 0; i < n; ++i) {
        res.push_back(distribution(generator));
    }
    return res;
}

std::vector<Geom::Triangle>
GetRandomTriangles (int tr_num) {
    std::vector<Geom::Triangle> trs;
    std::vector<int> p = IntGenerator(9 * tr_num, 0, MAXCOORD);
    for (unsigned i = 0; i < tr_num; ++i) {
        unsigned begin = i * 9;
        auto  x1 = static_cast<float>(p[begin]),
              y1 = static_cast<float>(p[begin + 1]),
              z1 = static_cast<float>(p[begin + 2]),
              x2 = static_cast<float>(p[begin + 3]),
              y2 = static_cast<float>(p[begin + 4]),
              z2 = static_cast<float>(p[begin + 5]),
              x3 = static_cast<float>(p[begin + 6]),
              y3 = static_cast<float>(p[begin + 7]),
              z3 = static_cast<float>(p[begin + 8]);

        Geom::Triangle tr {i, x1, y1, z1, x2, y2, z2, x3, y3, z3};
        trs.push_back(tr);
    }
    return trs;
}

std::unordered_map<unsigned, std::vector<unsigned>>
GetAnswer (std::vector<Geom::Triangle> &trs) {
    std::unordered_map<unsigned, std::vector<unsigned>> ans_table;

    for (auto i: trs)
        for (auto j: trs)
            if (j.number > i.number)
                if (i.IsIntersectWithOther(j))
                    ans_table[i.number].push_back(j.number);

    return ans_table;
}

void DumpAnswer (std::unordered_map<unsigned, std::vector<unsigned>>& ans_table) {
    for (auto& i: ans_table) {
        std::cout << "Triangle №" << i.first << " intersects with:" << std::endl;
        for (auto j: i.second) {
            std::cout << "\t" << j << ";" << std::endl;
        }
    }
}

void DumpIntersectionsWithTriangle (Geom::Triangle tr, std::vector<Geom::Triangle> &trs) {
    std::cout << "Triangle №" << tr.number << ":\n";
    for (auto& i: trs)
        if (tr.IsIntersectWithOther(i))
            std::cout << i.number << std::endl;
}