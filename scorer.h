#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

using StudentName = std::string;
using TaskName = std::string;

using ScoreTable = std::map<StudentName, std::set<TaskName>>;

class Scorer {
private:
    class StudentTaskState {
    public:
        bool is_checked_;
        uint8_t merge_request_count_;

        StudentTaskState();
        StudentTaskState(bool is_checked, uint8_t merge_request_count);
        void OnCheckFailed();
        void OnCheckSuccess();
        void OnMergeRequestOpen();
        void OnMergeRequestClosed();

        bool IsDefault() const;
        bool IsDone() const;
    };

    std::unordered_map<StudentName, std::unordered_map<TaskName, StudentTaskState>> data_;

    using Updater = std::function<void(StudentTaskState&)>;

    void StudentTaskUpdate(const StudentName& student_name, const TaskName& task_name, Updater updater);

public:
    void OnCheckFailed(const StudentName& student_name, const TaskName& task_name);
    void OnCheckSuccess(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name);
    void Reset();

    ScoreTable GetScoreTable() const;
};
