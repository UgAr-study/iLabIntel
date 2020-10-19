#include <algorithm>
#include <fstream>
#include "GeometryHeader.h"
#include "AlgorithmHeader.h"
//#include "Test/TestHeader.h"

std::vector<Geom::Triangle>
GetTriangles (std::vector<float>& p) {
    std::vector<Geom::Triangle> trs;
    for (unsigned i = 0; i < p.size() / 9; ++i) {
        unsigned begin = i * 9;

        Geom::Triangle tr {i, p[begin],     p[begin + 1], p[begin + 2],
                              p[begin + 3], p[begin + 4], p[begin + 5],
                              p[begin + 6], p[begin + 7], p[begin + 8]};
        trs.push_back(tr);
    }
    return trs;
}

int main() {
    int N;
    std::cin >> N;
    std::vector<float> dots;
    dots.reserve(9 * N);
    for (int i = 0 ; i < 9 * N; ++i) {
        float dot;
        std::cin >> dot;
        dots.push_back(dot);
    }

    std::vector<Geom::Triangle> triangles = GetTriangles(dots);

    /*
    std::vector<unsigned> nx2answer = GetAnswer(triangles);

    std::cout << "N^2 algorithm answer { ";
    for (auto i: nx2answer)
        std::cout << i << " ";
    std::cout << "}" << std::endl;
    */

    std::vector<unsigned> answer = Alg::FindIntersections(triangles);
    std::sort (answer.begin(), answer.end());


    //std::cout << "Octree alg answer { ";
    for (auto i: answer)
        std::cout << i << " ";
    //std::cout << "}" << std::endl;
    std::cout << std::endl;

    return 0;
}
