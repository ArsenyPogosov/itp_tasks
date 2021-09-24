#include <deque>
#include "tests_checking.h"

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {

    std::deque<std::string> students_order;
    for (auto& i : student_actions) {
        if (i.side == Side::Bottom) {
            students_order.push_back(i.name);
        }
        if (i.side == Side::Top) {
            students_order.push_front(i.name);
        }
    }

    std::vector<std::string> res;
    for (auto& i : queries) {
        res.push_back(students_order[i - 1]);
    }

    return res;
}
