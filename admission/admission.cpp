#include <algorithm>
#include <unordered_map>
#include "admission.h"

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> ord;
    for (auto& i : applicants) {
        ord.push_back(&i);
    }

    std::sort(ord.begin(), ord.end(), [](const Applicant* left, const Applicant* right) {
        auto make_comparable = [](const Applicant* a) {
            int reversed_points = -a->points;  // To sort by points in decreasing order
            return std::tie(reversed_points, a->student.birth_date.year, a->student.birth_date.month,
                            a->student.birth_date.day, a->student.name);
        };
        return make_comparable(left) < make_comparable(right);
    });

    std::unordered_map<std::string, const University*> university_from_name;
    for (auto& i : universities) {
        university_from_name[i.name] = &i;
    }

    AdmissionTable res;
    for (auto& i : universities) {
        res[i.name] = {};
    }

    for (auto& i : ord) {
        for (auto& j : i->wish_list) {
            if (res[j].size() < university_from_name[j]->max_students) {
                res[j].push_back(&i->student);
                break;
            }
        }
    }

    for (auto& i : res) {
        std::sort(i.second.begin(), i.second.end(), [](const Student* left, const Student* right) {
            auto make_comparable = [](const Student* s) {
                return std::tie(s->name, s->birth_date.year, s->birth_date.month, s->birth_date.day);
            };
            return make_comparable(left) < make_comparable(right);
        });
    }

    return res;
}
