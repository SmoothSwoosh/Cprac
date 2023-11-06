#include "temperature.h"
#include "schedule.h"
#include "mutation.h"
#include "simulated_annealing.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <string>

int main()
{
    srand(time(NULL));

    int n = 5;
    for (int i = 0; i < 200; ++i) {
        std::cout << "No: " << i << std::endl;
        double average_time = 0;
        for (int j = 0; j < n; ++j) {
            BoltzmannTemperature temperature;
            temperature.set(1000000);
            Schedule schedule(std::to_string(i) + ".txt");
            Mutation mutation;
            Annealing<Schedule, Mutation, BoltzmannTemperature> algo(schedule, mutation, temperature);
            auto start = std::chrono::high_resolution_clock::now();
            algo.start();
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> ms_double = end - start;
            std::cout << algo.get_best_schedule().get_quality() << std::endl;
            average_time += ms_double.count();
        }
        average_time /= n;
        std::cout << "Average time: " << average_time << "\n\n";
    }

    return 0;
}