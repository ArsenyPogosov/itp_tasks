#include "sort_students.h"

std::pair<int, std::pair<int, int>> GetCompareableDate(Student& a)
{
    return {a.birth_date.year, {a.birth_date.month, a.birth_date.day}};
}

bool DateSort(Student& a, Student& b) {
    if (GetCompareableDate(a) != GetCompareableDate(b))
        return GetCompareableDate(a) < GetCompareableDate(b);

    if (a.last_name != b.last_name)
        return a.last_name < b.last_name;

    return a.name < b.name;
}

bool NameSort(Student& a, Student& b) {
    if (a.last_name != b.last_name)
        return a.last_name < b.last_name;

    if (a.name != b.name)
        return a.name < b.name;

    return GetCompareableDate(a) < GetCompareableDate(b);
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    if (sortKind == SortKind::Date)
        sort(students.begin(), students.end(), DateSort);
    if (sortKind == SortKind::Name)
        sort(students.begin(), students.end(), NameSort);
}
