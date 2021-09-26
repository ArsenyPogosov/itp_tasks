#include <algorithm>
#include <unordered_map>
#include "admission.h"

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> applicants_ordered_by_priority;
    for (auto& applicant : applicants) {
        applicants_ordered_by_priority.push_back(&applicant);
    }

    std::sort(applicants_ordered_by_priority.begin(), applicants_ordered_by_priority.end(),
              [](const Applicant* left_compared, const Applicant* right_compared) {
                  auto make_comparable = [](const Applicant* applicant_to_convert) {
                      int reversed_points = -applicant_to_convert->points;  // To sort by points in decreasing order
                      return std::tie(reversed_points, applicant_to_convert->student.birth_date.year,
                                      applicant_to_convert->student.birth_date.month,
                                      applicant_to_convert->student.birth_date.day, applicant_to_convert->student.name);
                  };
                  return make_comparable(left_compared) < make_comparable(right_compared);
              });

    std::unordered_map<std::string, const University*> university_from_name;
    for (auto& university : universities) {
        university_from_name[university.name] = &university;
    }

    AdmissionTable resulting_admission_table;
    for (auto& university : universities) {
        resulting_admission_table[university.name] = {};
    }

    for (auto& applicant_pointer : applicants_ordered_by_priority) {
        for (auto& wished_university_name : applicant_pointer->wish_list) {
            if (resulting_admission_table[wished_university_name].size() <
                university_from_name[wished_university_name]->max_students) {
                resulting_admission_table[wished_university_name].push_back(&applicant_pointer->student);
                break;
            }
        }
    }

    for (auto& university_name : resulting_admission_table) {
        std::sort(university_name.second.begin(), university_name.second.end(),
                  [](const Student* left_compared, const Student* right_compared) {
                      auto make_comparable = [](const Student* student_to_convert_pointer) {
                          return std::tie(student_to_convert_pointer->name, student_to_convert_pointer->birth_date.year,
                                          student_to_convert_pointer->birth_date.month,
                                          student_to_convert_pointer->birth_date.day);
                      };
                      return make_comparable(left_compared) < make_comparable(right_compared);
                  });
    }

    return resulting_admission_table;
}
