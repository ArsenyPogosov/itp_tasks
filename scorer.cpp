#include "scorer.h"

Scorer::StudentTaskState::StudentTaskState() {
    is_checked_ = false;
    merge_request_count_ = 0;
}

Scorer::StudentTaskState::StudentTaskState(bool is_checked, uint8_t merge_request_count) {
    is_checked_ = is_checked;
    merge_request_count_ = merge_request_count;
}

void Scorer::StudentTaskState::OnCheckFailed() {
    is_checked_ = false;
}

void Scorer::StudentTaskState::OnCheckSuccess() {
    is_checked_ = true;
}

void Scorer::StudentTaskState::OnMergeRequestOpen() {
    ++merge_request_count_;
}

void Scorer::StudentTaskState::OnMergeRequestClosed() {
    --merge_request_count_;
}

bool Scorer::StudentTaskState::IsDefault() const {
    static StudentTaskState default_task_state = StudentTaskState();
    return (is_checked_ == default_task_state.is_checked_) &&
           (merge_request_count_ == default_task_state.merge_request_count_);
}
bool Scorer::StudentTaskState::IsDone() const {
    static StudentTaskState done_task_state = StudentTaskState(true, 0);
    return (is_checked_ == done_task_state.is_checked_) &&
           (merge_request_count_ == done_task_state.merge_request_count_);
}

void Scorer::StudentTaskUpdate(const StudentName& student_name, const TaskName& task_name, Updater updater) {
    auto student_data = data_.try_emplace(student_name).first;
    auto student_task_data = student_data->second.try_emplace(task_name).first;
    updater(student_task_data->second);
    if (student_task_data->second.IsDefault()) {
        student_data->second.erase(student_task_data->first);
    }
    if (student_data->second.empty()) {
        data_.erase(student_data->first);
    }
}

void Scorer::OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
    StudentTaskUpdate(student_name, task_name, [](StudentTaskState& state) { state.OnCheckFailed(); });
}

void Scorer::OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
    StudentTaskUpdate(student_name, task_name, [](StudentTaskState& state) { state.OnCheckSuccess(); });
}

void Scorer::OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
    StudentTaskUpdate(student_name, task_name, [](StudentTaskState& state) { state.OnCheckSuccess(); });
}

void Scorer::OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
    StudentTaskUpdate(student_name, task_name, [](StudentTaskState& state) { state.OnMergeRequestClosed(); });
}

void Scorer::Reset() {
    data_.clear();
}

ScoreTable Scorer::GetScoreTable() const {
    ScoreTable result;
    for (auto& [student_name, student_task_states] : data_) {
        for (auto& [task_name, student_task_state] : student_task_states) {
            if (student_task_state.IsDone()) {
                result[student_name].insert(task_name);
            }
        }
    }

    return result;
}
