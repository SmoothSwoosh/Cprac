#ifndef MUTATION_H
#define MUTATION_H

#include "schedule.h"

template<typename ScheduleT>
class AbstractMutation {
public:
    virtual ScheduleT mutate(ScheduleT schedule) = 0;
};

class Mutation : public AbstractMutation<Schedule> {
public:
    Mutation() {}
    virtual Schedule mutate(Schedule schedule) override {
        if (schedule.get_proc_num() == 1) {
            return schedule;
        }
        // move random task from one random processor to the end of another
        long long proc1 = rand() % schedule.get_proc_num(); // first processor
        while (schedule.get_proc_task_num(proc1) == 0) {
            // find busy proc
            proc1 = rand() % schedule.get_proc_num();
        }

        long long proc2 = rand() % schedule.get_proc_num(); // second processor
        while (proc1 == proc2 && schedule.get_proc_num() > 1) {
            // processors should be different
            proc2 = rand() % schedule.get_proc_num();
        }

        long long task_idx = rand() % schedule.get_proc_task_num(proc1); // get random task from proc1

        // move this task to proc2
        schedule.transfer_task(task_idx, proc1, proc2);

        return schedule;
    };
};

#endif