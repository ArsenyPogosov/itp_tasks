#include <algorithm>
#include <unordered_map>
#include "admission.h"

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> applicants_ordered_by_priority;
    for (const auto& applicant : applicants) {
        applicants_ordered_by_priority.push_back(&applicant);
    }

    std::sort(applicants_ordered_by_priority.begin(), applicants_ordered_by_priority.end(),
              [](const Applicant* const left_compared, const Applicant* const right_compared) {
                  const auto make_comparable = [](const Applicant* const applicant_to_convert) {
                      int reversed_points = -applicant_to_convert->points;  // To sort by points in decreasing order
                      return std::tie(reversed_points, applicant_to_convert->student.birth_date.year,
                                      applicant_to_convert->student.birth_date.month,
                                      applicant_to_convert->student.birth_date.day, applicant_to_convert->student.name);
                  };
                  return make_comparable(left_compared) < make_comparable(right_compared);
              });

    std::unordered_map<std::string, const University*> university_from_name;
    for (const auto& university : universities) {
        university_from_name[university.name] = &university;
    }

    AdmissionTable resulting_admission_table;
    for (const auto& university : universities) {
        resulting_admission_table[university.name] = {};
    }

    for (const auto& applicant_pointer : applicants_ordered_by_priority) {
        for (const auto& wished_university_name : applicant_pointer->wish_list) {
            if (resulting_admission_table[wished_university_name].size() <
                university_from_name[wished_university_name]->max_students) {
                resulting_admission_table[wished_university_name].push_back(&applicant_pointer->student);
                break;
            }
        }
    }

    for (auto& [university_name, university_students] : resulting_admission_table) {
        std::sort(university_students.begin(), university_students.end(),
                  [](const Student* const left_compared, const Student* const right_compared) {
                      const auto make_comparable = [](const Student* const student_to_convert_pointer) {
                          return std::tie(student_to_convert_pointer->name, student_to_convert_pointer->birth_date.year,
                                          student_to_convert_pointer->birth_date.month,
                                          student_to_convert_pointer->birth_date.day);
                      };
                      return make_comparable(left_compared) < make_comparable(right_compared);
                  });
    }

    return resulting_admission_table;
}
