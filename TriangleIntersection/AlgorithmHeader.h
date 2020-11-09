#pragma once

#include <unordered_map>
#include <stack>
#include <utility>
#include <vector>
#include <random>
#include "GeometryHeader.h"
#include <assert.h>

enum {MAXDEPTH = 5, NUMINCUBE = 20};

namespace Alg {

    struct Cube;
    struct node_t;
    struct Octree;

    using res_table = std::unordered_map<Geom::Triangle*, Geom::Triangle>;
    using triangle_vec = std::vector<Geom::Triangle>;
    using triangles_ptr = std::vector<Geom::Triangle*>;
    using special_trs = std::unordered_map<Geom::Triangle*, Cube*>;

    struct Cube {
    private:
        Geom::Point left_bot, right_top;
        unsigned number_of_elems = 0;
        Cube* parent_cube;
        triangles_ptr triangles;

    public:
        unsigned getNumberOfElems () const { return number_of_elems; }

        Cube () : left_bot(), right_top() { parent_cube = nullptr; }
        explicit Cube(Geom::Point leftBot, Geom::Point rightTop ,
                      triangles_ptr& triangle, special_trs & spec_trs, Cube* parent);
        ~Cube() = default;

        signed char IsTriangleInCube (Geom::Triangle triangle) const;
        void CheckEveryTriangleInCube (res_table& ans_table) const;
        void CheckThisTriangle (Geom::Triangle *triangle, res_table& ans_table) const;
        Cube* CubeFraction (special_trs& spec_trs);
    };

    struct node_t {
        node_t* parent = nullptr;
        Cube cube_;
        node_t *childs[8] = { nullptr };

        explicit node_t(Cube cb, node_t* parent_) : parent(parent_) { cube_ = std::move(cb); }
        ~node_t() { for (auto i: childs) delete i; }
    };

    struct Octree {

    public:
        explicit Octree(unsigned MaxDepth, unsigned num_elem_in_cube, triangle_vec& triangles);
        ~Octree() { delete top_; };

        Octree& operator= (const Octree& t) = delete;
        Octree(const Octree& t) = delete;

        void CheckLeafs (res_table& ans_table);
        void CheckSpecialTriangles (res_table& ans_table);

    private:
        node_t *top_;
        unsigned maxDepth, max_num_of_elems_in_cube;
        special_trs spec_trs;

        node_t* BuildTree (triangle_vec& triangles);
        void BuildBranches (node_t* top, unsigned level);
    };

    std::vector<unsigned> FindIntersections (triangle_vec& triangles);

    bool DefineCube(const triangle_vec& triangles, Geom::Point *dots);

}

