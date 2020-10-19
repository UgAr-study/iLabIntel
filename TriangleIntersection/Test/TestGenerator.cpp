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

std::vector<unsigned>
GetAnswer (std::vector<Geom::Triangle> &trs) {
    std::unordered_map<unsigned, int> ans_table;
    std::vector<unsigned> res;
    for (auto i: trs)
        for (auto j: trs)
            if (j.number != i.number)
                if (i.IsIntersectWithOther(j)) {
                    ans_table[i.number] = 1;
                    ans_table[j.number] = 1;
                }

    res.reserve(ans_table.size());
    for (auto i: ans_table)
        res.push_back(i.first);

    return res;
}

/*
int main () {

    Geom::Point A11 (6, 6, 3),
                A12 (10, 10, 2),
                A13 (7, 10, 6);
    Geom::Point A21 (9, 5, 8),
                A22 (5, 10, 3),
                A23 (7, 0, 8);


    Geom::Triangle tr1 {1, A11, A12, A13},
                   tr2 {2, A21, A22, A23};
    std::vector<Geom::Triangle> stupidtrs {tr1, tr2};

    /*
    if (tr1.IsIntersectWithOther(tr2))
        std::cout << "Yes\n";
    else
        std::cout << "No\n";


    std::vector<Geom::Triangle> triangles = GetRandomTriangles(NTRS);
    std::vector<unsigned> Right_Answer = GetAnswer(triangles),
                          Alg_Answer = Alg::FindIntersections (triangles);

    std::sort (Right_Answer.begin(), Right_Answer.end());
    std::sort (Alg_Answer.begin(), Alg_Answer.end());

    std::cout << "Right_Answer is { ";
    for (auto i: Right_Answer)
        std::cout << i << " ";
    std::cout << "}" << std::endl;

    std::cout << "Alg_Answer is   { ";
    for (auto i: Alg_Answer)
        std::cout << i << " ";
    std::cout << "}" << std::endl;

    //for (auto tr: triangles)
    //    std::cout << tr << std::endl;
    return 0;
}*/