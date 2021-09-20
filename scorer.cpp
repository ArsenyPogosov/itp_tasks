#include <unordered_map>
#include "scorer.h"

struct TaskProfile {
    bool is_checked;
    int last_check_time;
    int merge_request_count;
    TaskProfile(bool is_checked = false, int last_check_time = -1, int merge_request_count = 0)
        : is_checked(is_checked), last_check_time(last_check_time), merge_request_count(merge_request_count) {
    }
};

bool IsDone(const TaskProfile& task) {
    return task.is_checked && (task.merge_request_count == 0);
}

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::unordered_map<StudentName, std::unordered_map<TaskName, TaskProfile>> counter;
    for (auto& i : events) {
        if (i.time <= score_time) {
            if (i.event_type == EventType::CheckFailed) {
                if (counter[i.student_name][i.task_name].last_check_time < i.time) {
                    counter[i.student_name][i.task_name].is_checked = false;
                    counter[i.student_name][i.task_name].last_check_time = i.time;
                }
            }
            if (i.event_type == EventType::CheckSuccess) {
                if (counter[i.student_name][i.task_name].last_check_time < i.time) {
                    counter[i.student_name][i.task_name].is_checked = true;
                    counter[i.student_name][i.task_name].last_check_time = i.time;
                }
            }
            if (i.event_type == EventType::MergeRequestClosed) {
                --counter[i.student_name][i.task_name].merge_request_count;
            }
            if (i.event_type == EventType::MergeRequestOpen) {
                ++counter[i.student_name][i.task_name].merge_request_count;
            }
        }
    }

    ScoreTable res;
    for (auto& i : counter) {
        for (auto& j : i.second) {
            if (IsDone(j.second)) {
                res[i.first].insert(j.first);
            }
        }
    }

    return res;
}
