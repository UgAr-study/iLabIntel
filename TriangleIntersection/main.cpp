#include <algorithm>
#include <fstream>
#include "GeometryHeader.h"
#include "AlgorithmHeader.h"

std::vector<Geom::Triangle>
GetTriangles (std::vector<int>& p) {
    std::vector<Geom::Triangle> trs;
    for (unsigned i = 0; i < p.size() / 9; ++i) {
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

void IntGenerator (int n_trs, int min, int max) {
    std::ofstream file;
    file.open("../Test/test.txt");

    std::mt19937 generator(time(nullptr));
    std::uniform_int_distribution<int> distribution(min, max);

    if (file.is_open()) {
        file << 9 * n_trs << " ";
        for (int i = 0; i < 9 * n_trs; ++i)
            file << distribution(generator) << " ";
    }
}

int main() {
    IntGenerator(300, 0, 50);

    int N;
    std::cin >> N;
    std::vector<int> dots;
    dots.reserve(9 * N);
    for (int i = 0 ; i < 9 * N; ++i) {
        int dot;
        std::cin >> dot;
        dots.push_back(dot);
    }

    std::vector<Geom::Triangle> triangles = GetTriangles(dots);
    std::vector<unsigned> answer = Alg::FindIntersections(triangles);
    std::sort (answer.begin(), answer.end());

    for (auto i: answer)
        std::cout << i << " ";
    std::cout << std::endl;
    for (auto i: answer)
        std::cout << triangles[i];
    return 0;
}
