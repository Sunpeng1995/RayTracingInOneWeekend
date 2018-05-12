#ifndef __HITABLELIST_H__
#define __HITABLELIST_H__
#include <vector>
#include "hitable.h"

class hitable_list : public hitable {
public:
    hitable_list() {}
    hitable_list(std::vector<hitable*> l) : list(l) {}
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &hr) const override;

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

#endif