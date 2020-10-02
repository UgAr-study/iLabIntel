#include "GeometryHeader.h"

int main() {
    Geom::Point p000{0, 0, 0}, p111{1, 1, 1}, p100{1, 0, 0},
                p010{0, 1, 0}, p001{0, 0, 1}, p110{1, 1, 0},
                p1_200{0.5, 0, 0};
    Geom::Triangle tr1{p000, p110, p111}, tr2{p000, p010, p100};
    if (tr1.IsIntersectWithOther(tr2))
        std::cout << "Yes";
    else
        std::cout << "No";
    return 0;
}
