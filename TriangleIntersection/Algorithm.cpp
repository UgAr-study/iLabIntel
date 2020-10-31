#include "AlgorithmHeader.h"
#include "GeometryHeader.h"

//////////////////
///////Cube///////
//////////////////

void Alg::Cube::CheckEveryTriangleInCube (res_table& ans_table) const {
    for (auto& i: triangles)
        for (auto& j: triangles) {
            if (j->number <= i->number) //j.number && i.number
                continue;
            if (i->IsIntersectWithOther(*j)) {
                ans_table[i] = *i;
                ans_table[j] = *j;
            }
        }
}

void Alg::Cube::CheckThisTriangle(Geom::Triangle *triangle, res_table& ans_table) const {
    for (auto& i: triangles)
        if ((i->number != triangle->number) && triangle->IsIntersectWithOther(*i)) {
            ans_table[i] = *i;
            ans_table[triangle] = *triangle;
        }
}

Alg::Cube::Cube(Geom::Point leftBot, Geom::Point rightTop, triangles_ptr& triangle,
                special_trs& spec_trs, Cube* parent) {

    parent_cube = parent;
    left_bot = leftBot;
    right_top = rightTop;
    for (auto& i: triangle) {
        signed char isInCube = IsTriangleInCube(*i);
        if (isInCube == 1) {
            triangles.push_back(i);
            number_of_elems++;
        } else if (isInCube == 0)
            spec_trs[i] = parent_cube;
    }
}

signed char Alg::Cube::IsTriangleInCube(Geom::Triangle triangle) const {
    Geom::Point p[3] {triangle.A1, triangle.A2, triangle.A3};
    bool b[3];
    for (int i = 0; i < 3; ++i) {
        if (    (p[i].x >= left_bot.x && p[i].x <= right_top.x) &&
                (p[i].y >= left_bot.y && p[i].y <= right_top.y) &&
                (p[i].z >= left_bot.z && p[i].z <= right_top.z)) {
            b[i] = true;
            continue;
        }
        b[i] = false;
    }

    if (b[0] && b[1] && b[2])
        return 1;
    if (b[0] || b[1] || b[2])
        return 0;

    return -1;
}

Alg::Cube*
Alg::Cube::CubeFraction (special_trs& spec_trs) {
    Geom::Point begin = left_bot, end = right_top;
    Geom::Point center = (begin + end) / 2;

    float edge = end.x - begin.x;

    Geom::Point px00{edge / 2, 0, 0},
                p0y0{0, edge / 2, 0},
                p00z{0, 0, edge / 2};

    Geom::Point beg_mid_x = begin + px00,
                beg_mid_y = begin + p0y0,
                beg_mid_z = begin + p00z,
                center_mid_bot   = center - p00z,
                center_mid_top   = center + p00z,
                center_mid_front = center - p0y0,
                center_mid_back  = center + p0y0,
                center_mid_left  = center - px00,
                center_mid_right = center + px00,
                end_mid_x = end - px00,
                end_mid_y = end - p0y0,
                end_mid_z = end - p00z;

    auto child_cubes = new Cube[8] {
            Cube {begin, center, triangles, spec_trs, this},
            Cube {beg_mid_x, center_mid_right, triangles, spec_trs, this},
            Cube {center_mid_bot, end_mid_z, triangles, spec_trs, this},
            Cube {beg_mid_y, center_mid_back, triangles, spec_trs, this},
            Cube {beg_mid_z, center_mid_top, triangles, spec_trs, this},
            Cube {center_mid_front, end_mid_y, triangles, spec_trs, this},
            Cube {center, end, triangles, spec_trs, this},
            Cube {center_mid_left, end_mid_x, triangles, spec_trs, this}};
    return child_cubes;
}

/*
bool Alg::Cube::IsOverlapWithInterval(Geom::Interval interval) const {
    //check intersection with square, then check signs of ends of the interval
    Geom::Point p000 (left_bot),
                p100 {right_top.x, left_bot.y, left_bot.z},
                p010 {left_bot.x, right_top.y, left_bot.z},
                p001 {left_bot.x, left_bot.y, right_top.z},
                p110 {right_top.x, right_top.y, left_bot.z},
                p101 {right_top.x, left_bot.y, right_top.z},
                p011 {left_bot.x, right_top.y, right_top.z},
                p111 (right_top);

    std::vector<Geom::Square> faces{ Geom::Square{p000, p110},
                                     Geom::Square{p000, p101},
                                     Geom::Square{p000, p011},
                                     Geom::Square{p100, p111},
                                     Geom::Square{p010, p111},
                                     Geom::Square{p001, p111} };

    std::vector<Geom::Point> inter_points_candidates;
    inter_points_candidates.reserve(6);
    Geom::Line line {interval.C1, interval.C2};
    for (auto i : faces)
        inter_points_candidates.push_back(i.IntersectionWithLine(line));
    std::vector<Geom::Point> inter_points;
    inter_points.reserve(2);
    int inter_points_[2];
    for (int i = 0, j = 0; i < 6; ++i) {
        if (inter_points_candidates[i].isValid()) {
            inter_points_[j++] = i;
        }
    }
    for (auto i: inter_points_candidates)
        if (i.isValid())
            inter_points.push_back(i);
    if (inter_points.empty())
        return false;

    return false;
}
*/

//////////////////
///////Octree/////
//////////////////

Alg::Octree::Octree(unsigned MaxDepth, unsigned num_elem_in_cube, triangle_vec& triangles) {
    maxDepth = MaxDepth;
    max_num_of_elems_in_cube = num_elem_in_cube;
    _top = BuildTree(triangles);
}

Alg::node_t*
Alg::Octree::BuildTree(triangle_vec& triangles) {

    Geom::Point main_cube[2];
    if (!DefineCube(triangles, main_cube)){
        std::cout << "Error, can't define cube" << std::endl;
        exit(EXIT_FAILURE);
    }

    triangles_ptr trs;

    for (int i = 0; i < triangles.size(); ++i) {
        trs.push_back(&triangles[i]);
    }

    auto res = new node_t{Cube{main_cube[0], main_cube[1], trs, spec_trs, nullptr}};
    if (res->cube_.getNumberOfElems() > max_num_of_elems_in_cube) {
        RecursiveBuild(res, 0);
    }

    return res;
}

void Alg::Octree::RecursiveBuild(Alg::node_t *cur_node, unsigned level) {

    if (cur_node->cube_.getNumberOfElems() < max_num_of_elems_in_cube || level >= maxDepth)
        return;

    Alg::Cube* child_cubes = cur_node->cube_.CubeFraction(spec_trs);

    for (int i = 0; i < 8; ++i) {
        cur_node->_child[i] = new node_t {child_cubes[i]};
        RecursiveBuild(cur_node->_child[i], level + 1);
    }
}

Alg::Octree::~Octree() {
    RecursiveDelete(_top);
}

void Alg::Octree::RecursiveDelete(Alg::node_t *top) {
    if (top->_child[0] == nullptr)
        return;

    for (auto & i : top->_child) {
        RecursiveDelete(i);
        delete i;
    }
}

//////////////////
/////Functions////
//////////////////

std::vector<unsigned>
Alg::FindIntersections (Alg::triangle_vec& triangles) {
    Alg::res_table answer_table;
    Alg::Octree octree{MAXDEPTH, NUMINCUBE, triangles};

    octree.RecursiveDescent(answer_table);

    octree.CheckSpecialTriangles(answer_table);

    std::vector<unsigned> numbers_of_intersection_trs;
    auto iter = answer_table.begin();
    for (; iter != answer_table.end(); ++iter)
        numbers_of_intersection_trs.push_back(iter->second.number);

    return numbers_of_intersection_trs;
}

void Alg::Octree::RecursiveDescent (Alg::node_t *top, Alg::res_table& ans_table) {
    if (top->_child[0] == nullptr) {
        top->cube_.CheckEveryTriangleInCube(ans_table);
        return;
    }

    for (auto & i : top->_child) {
        if (i != nullptr)
            RecursiveDescent(i, ans_table);
    }
}

void Alg::Octree::RecursiveDescent(res_table &ans_table) {
    RecursiveDescent(_top, ans_table);
}

void Alg::Octree::CheckSpecialTriangles(Alg::res_table& ans_table) {
    for (auto& i: spec_trs)
        i.second->CheckThisTriangle(i.first, ans_table);
}

bool Alg::DefineCube(const Alg::triangle_vec& triangles, Geom::Point *dots) {
    if (triangles.empty())
        return false;

    Geom::Triangle ref_tr = triangles[0];
    float min_x = ref_tr.A1.x, max_x = ref_tr.A1.x, min_y = ref_tr.A1.y, max_y = ref_tr.A1.y, min_z = ref_tr.A1.z, max_z = ref_tr.A1.z;
    for (auto tr: triangles) {
        Geom::Point points[3] {tr.A1, tr.A2, tr.A3};
        for (auto p: points) {
            if (p.x < min_x)
                min_x = p.x;
            if (p.x > max_x)
                max_x = p.x;
            if (p.y < min_y)
                min_y = p.y;
            if (p.y > max_y)
                max_y = p.y;
            if (p.z < min_z)
                min_z = p.z;
            if (p.z > max_z)
                max_z = p.z;
        }
    }

    float x_length = max_x - min_x, y_length = max_y - min_y, z_length = max_z - min_z;

    float max_length = std::max(x_length, y_length);
    max_length = std::max(max_length, z_length);

    dots[0].x = min_x;
    dots[0].y = min_y;
    dots[0].z = min_z;

    dots[1].x = min_x + max_length;
    dots[1].y = min_y + max_length;
    dots[1].z = min_z + max_length;

    return true;
}