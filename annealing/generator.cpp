#include <random>
#include <fstream>
#include <iostream>
#include <chrono>
#include <string>
#include <map>

int main(int argc, char *argv[]) {
    srand(time(NULL));

    long long low_time, upper_time;
    std::cin >> low_time >> upper_time;

    long long proc_nums[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    long long task_nums[20] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
                               1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000};

    int i = 0;
    for (auto proc_num : proc_nums) {
        for (auto task_num : task_nums) {
            std::ofstream file("input/" + std::to_string(i) + ".csv");
            file << proc_num << "\n";
            file << task_num << "\n";

            for (long long i = 0; i < task_num; ++i) {
                file << i << "," << (rand() % (upper_time - low_time + 1)) + low_time << "\n";
            }
            ++i;
        }
    }

    return 0;
}