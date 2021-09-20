#include <algorithm>
#include <unordered_map>
#include "admission.h"

std::pair<int, std::pair<int, int>> GetComparableDate(const Date& date) {
    return {date.year, {date.month, date.day}};
}

bool PriorityComp(const Applicant* a, const Applicant* b) {
    if (a->points != b->points)
        return (a->points > b->points);
    auto aDate = GetComparableDate(a->student.birth_date), bDate = GetComparableDate(b->student.birth_date);
    if (aDate != bDate)
        return (aDate < bDate);
    if (a->student.name < b->student.name)
        return (a->student.name < b->student.name);

    return false;
}

bool AlphabetComp(const Student* a, const Student* b) {
    if (a->name != b->name)
        return (a->name < b->name);
    auto aDate = GetComparableDate(a->birth_date), bDate = GetComparableDate(b->birth_date);
    if (aDate != bDate)
        return (aDate < bDate);

    return false;
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> ord;
    for (auto& i : applicants)
        ord.push_back(&i);
    std::sort(ord.begin(), ord.end(), PriorityComp);

    std::unordered_map<std::string, const University*> universityFromName;
    for (auto& i : universities)
        universityFromName[i.name] = &i;

    AdmissionTable res;
    for (auto& i : universities)
        res[i.name] = {};

    for (auto& i : ord)
        for (auto& j : i->wish_list)
            if (res[j].size() < universityFromName[j]->max_students) {
                res[j].push_back(&i->student);
                break;
            }

    for (auto& i : res)
        std::sort(i.second.begin(), i.second.end(), AlphabetComp);

    return res;
}
