#pragma once
#include <vector>
#include <iterator>
#include "hitable.h"

class bvh_node : public hitable {
public:
    bvh_node() {}
    bvh_node(std::vector<hitable*>::iterator begin, int n, float t0, float t1);
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &hr) const;
    virtual bool bounding_box(float t0, float t1, aabb &box) const;
private:
    hitable *left;
    hitable *right;
    aabb box;
};

bvh_node::bvh_node(std::vector<hitable*>::iterator begin, int n, float t0, float t1) {
    int axis = int(3 * drand48());
    if (axis == 0)
        std::sort(begin, begin + n, [](hitable *a, hitable *b) {
            aabb box_left, box_right;
            if (!a->bounding_box(0, 0, box_left) || !b->bounding_box(0, 0, box_right))
                std::cerr << "no boungding box in bvh_node constructor" << std::endl;
            return box_left.min().x() < box_right.min().x();
        });
    else if (axis == 1)
        std::sort(begin, begin + n, [](hitable *a, hitable *b) {
            aabb box_left, box_right;
            if (!a->bounding_box(0, 0, box_left) || !b->bounding_box(0, 0, box_right))
                std::cerr << "no boungding box in bvh_node constructor" << std::endl;
            return box_left.min().y() < box_right.min().y();
        });
    else
        std::sort(begin, begin + n, [](hitable *a, hitable *b) {
            aabb box_left, box_right;
            if (!a->bounding_box(0, 0, box_left) || !b->bounding_box(0, 0, box_right))
                std::cerr << "no boungding box in bvh_node constructor" << std::endl;
            return box_left.min().z() < box_right.min().z();
        });
    if (n == 1) {
        left = right = *begin;
    }
    else if (n == 2) {
        left = *begin;
        right = *(begin + 1);
    }
    else {
        left = new bvh_node(begin, n / 2, t0, t1);
        right = new bvh_node(begin + n / 2, n - n / 2, t0, t1);
    }

    aabb left_box, right_box;
    if (!left->bounding_box(t0, t1, left_box) || !right->bounding_box(t0, t1, right_box))
        std::cerr << "no boungding box in bvh_node constructor" << std::endl;
    box = aabb::surrounding_box(left_box, right_box);
}

bool bvh_node::hit(const ray &r, float t_min, float t_max, hit_record &hr) const {
    if (box.hit(r, t_min, t_max)) {
        hit_record left_hr, right_hr;
        bool hit_left = left->hit(r, t_min, t_max, left_hr);
        bool hit_right = right->hit(r, t_min, t_max, right_hr);
        if (hit_left && hit_right) {
            if (left_hr.t < right_hr.t)
                hr = left_hr;
            else
                hr = right_hr;
            return true;
        }
        else if (hit_left) {
            hr = left_hr;
            return true;
        }
        else if (hit_right) {
            hr = right_hr;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool bvh_node::bounding_box(float t0, float t1, aabb &b) const {
    b = box;
    return true;
}