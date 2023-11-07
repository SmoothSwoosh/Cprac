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

    int n = 20;
    for (int i = 1; i <= n; ++i) {
		std::cout << "Nproc: " << i << std::endl;
		BoltzmannTemperature temperature;
		temperature.set(1000000);
		Schedule schedule("parallel_input/" + std::to_string(i) + ".csv");
		Mutation mutation;
		auto start = std::chrono::high_resolution_clock::now();
		auto sch = ParallelAnnealing<Schedule, Mutation, BoltzmannTemperature>(i, schedule, mutation, temperature);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> ms_double = end - start;
		std::cout << sch.get_quality() << std::endl;
		std::cout << ms_double.count() << std::endl;
	}

    return 0;
}
