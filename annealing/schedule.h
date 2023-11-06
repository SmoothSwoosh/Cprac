#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "iostream"
#include "assert.h"

class AbstractSchedule {
public:
    virtual long long get_quality() const = 0;
};

class Schedule : AbstractSchedule {
    long long _proc_num = 0;
    long long _task_num = 0;
    std::vector<long long> _task_time{};
    std::vector<long long> _task_to_proc{};
    std::vector<std::vector<long long>> _proc_to_task{};
public:
    Schedule() {}
    Schedule(std::string filename);
    virtual long long get_quality() const override;
    void transfer_task(long long task, long long proc_from, long long proc_to);
    long long get_proc_num() const;
    long long get_task_num() const;
    long long get_proc_task_num(long long proc) const;
    std::string repr() const;
};

long long Schedule::get_proc_num() const {
    return _proc_num;
}

long long Schedule::get_task_num() const {
    return _task_num;
}

long long Schedule::get_proc_task_num(long long proc) const {
    return _proc_to_task[proc].size();
}

std::string Schedule::repr() const {
    std::stringstream ss;
    long long proc = 0;
    for (const auto& proc_schedule : _proc_to_task) {
        ss << proc << ":";
        for (long long task_idx = 0; task_idx < proc_schedule.size(); ++task_idx) {
            ss << proc_schedule[task_idx];
            if (task_idx + 1 < proc_schedule.size()) {
                ss << ",";
            }
        }
        if (proc + 1 < _proc_num) {
            ss << "\n";
        }
        ++proc;
    }
    return ss.str();
}

void Schedule::transfer_task(long long task_idx, long long proc_from, long long proc_to) {
    // delete task from first proc
    long long task = _proc_to_task[proc_from][task_idx]; // task number
    _proc_to_task[proc_from].erase(_proc_to_task[proc_from].begin() + task_idx);

    // add task to the end of second proc
    _proc_to_task[proc_to].push_back(task);

    _task_to_proc[task] = proc_to; // update task -> proc mapping
}

Schedule::Schedule(std::string filename) {
    std::ifstream file(filename);
    std::stringstream ss;
    std::string line;

    if (file.is_open()) {
        file >> line;
        ss = std::stringstream(line);
        ss >> _proc_num; // read proc_num
        file >> line;
        ss = std::stringstream(line);
        ss >> _task_num; // read task_num

        _proc_to_task.resize(_proc_num);
        _task_time.resize(_task_num);
        _task_to_proc.resize(_task_num);
        
        long long task, time;
        while (file >> line) {
            long long delimiter = line.find(',');
            std::stringstream new_ss;
            new_ss << line.substr(0, delimiter) << " " << line.substr(delimiter + 1, line.size() - delimiter);
            new_ss >> task >> time;
            _task_time[task] = time;
            long long proc = rand() % _proc_num;
            _proc_to_task[proc].push_back(task);
            _task_to_proc[task] = proc;
        }
    }

    file.close();
}

long long Schedule::get_quality() const {
    long long quality = 0;
    for (const auto& proc_schedule : _proc_to_task) {
        long long start_time = 0;
        for (long long task : proc_schedule) {
            quality += start_time + _task_time[task];
            start_time += _task_time[task];
        }
    }
    return quality;
}

#endif