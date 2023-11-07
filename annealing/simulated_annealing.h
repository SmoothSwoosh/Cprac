#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <random>
#include <mutex>
#include <thread>
#include <vector>

template<typename ScheduleT, typename MutationT, typename TemperatureT>
class Annealing {
    ScheduleT _current_schedule;
    ScheduleT _best_schedule;
    MutationT _mutation;
    TemperatureT _temperature;
    long long _best_iteration = 0;
    long long _limit = 100;
public:
    Annealing(ScheduleT schedule, MutationT mutation, TemperatureT temperature) {
        _current_schedule = schedule;
        _best_schedule = schedule;
        _mutation = mutation;
        _temperature = temperature;
    };

    void start() {
        long long iteration = 0;
        while (iteration - _best_iteration <= _limit) {
            ScheduleT new_schedule = _mutation.mutate(_current_schedule);
            double delta = new_schedule.get_quality() - _current_schedule.get_quality();
            if (_best_schedule.get_quality() - new_schedule.get_quality() > 0) {
                _best_schedule = new_schedule;
                _best_iteration = iteration;
                _current_schedule = new_schedule;
            } else if (delta <= 0) {
                _current_schedule = new_schedule;
            } else {
                double threshold = exp(-delta / _temperature.get());
                if ((double)rand() / (double)RAND_MAX > threshold) {
                    _current_schedule = new_schedule;
                }
            }
            _temperature.decrease();
            ++iteration;
        }
    }

    void parallel_start(ScheduleT& parallel_best_schedule, std::mutex& mutex) {
         start();
         mutex.lock();
         if (parallel_best_schedule.get_quality() - _best_schedule.get_quality() > 0) {
             parallel_best_schedule = _best_schedule;
         }
         mutex.unlock();
    }

    ScheduleT get_best_schedule() const {
        return _best_schedule;
    }
};

template<typename ScheduleT, typename MutationT, typename TemperatureT>
ScheduleT ParallelAnnealing(long long Nproc, ScheduleT initial_schedule, MutationT mutation, 
                            TemperatureT temperature, long long parallel_limit = 10) {
    std::mutex mutex;
    ScheduleT parallel_best_schedule = initial_schedule;
    long long iteration = 0;
    long long best_iteration = 0;

    while (iteration - best_iteration <= parallel_limit) {
        std::vector<Annealing<ScheduleT, MutationT, TemperatureT>> models;
        std::vector<std::thread> threads;

        for (long long i = 0; i < Nproc; ++i) {
            models.push_back(Annealing<ScheduleT, MutationT, TemperatureT>(initial_schedule, mutation, temperature));
        }

        for (long long i = 0; i < Nproc; ++i) {
            auto thread_perform = [&mutex, &models, &parallel_best_schedule, i] {
				models[i].parallel_start(parallel_best_schedule, mutex);
			};
            threads.push_back(std::thread{thread_perform});
        }

        for (long long i = 0; i < Nproc; ++i) {
            threads[i].join();
        }

        if (initial_schedule.get_quality() - parallel_best_schedule.get_quality() < 0) {
            // parallel schedule is better than initial one
            // change initial schedule to parallel schedule
            initial_schedule = parallel_best_schedule;
            best_iteration = iteration;
        }

        ++iteration;
    }
    
    return parallel_best_schedule;
}

#endif
