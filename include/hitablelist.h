#pragma once
#include <vector>
#include "hitable.h"

class hitable_list : public hitable {
public:
    hitable_list() {}
    hitable_list(std::vector<hitable*> l) : list(l) {}
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &hr) const override;
    virtual bool bounding_box(float t0, float t1, aabb &box) const override;

private:
    std::vector<hitable*> list;
};

bool hitable_list::hit(const ray &r, float t_min, float t_max, hit_record &hr) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closet_so_far = t_max;
    for (auto i : list) {
        if (i->hit(r, t_min, closet_so_far, temp_rec)) {
            hit_anything = true;
            closet_so_far = temp_rec.t;
            hr = temp_rec;
        }
    }

    return hit_anything;
}

bool hitable_list::bounding_box(float t0, float t1, aabb &box) const {
    if (list.size() < 1)
        return false;
    aabb tempbox;
    bool first = list[0]->bounding_box(t0, t1, tempbox);
    if (!first)
        return false;
    else
        box = tempbox;

    for (int i = 0; i < list.size(); i++) {
        if (list[i]->bounding_box(t0, t1, tempbox)) {
            box = aabb::surrounding_box(box, tempbox);
        }
        else
            return false;
    }
    return true;
}