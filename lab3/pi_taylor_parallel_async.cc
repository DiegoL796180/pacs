#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <future> // Necesario para std::async y std::future

using my_float = long double;

my_float
pi_taylor_chunk(size_t start_step, size_t stop_step) {
    my_float local_sum = 0.0;
    for (size_t i = start_step; i < stop_step; i++) {
        my_float term = 1.0 / (2.0 * i + 1.0);
        if (i % 2 == 0) { // n par
            local_sum += term;
        }
        else { // n impar
            local_sum -= term;
        }
    }
    return local_sum;
}

std::pair<size_t, size_t>
usage(int argc, const char* argv[]) {
    // read the number of steps from the command line
    if (argc != 3) {
        std::cerr << "Invalid syntax: pi_taylor <steps> <threads>" << std::endl;
        exit(1);
    }

    size_t steps = std::stoll(argv[1]);
    size_t threads = std::stoll(argv[2]);

    if (steps < threads) {
        std::cerr << "The number of steps should be larger than the number of threads" << std::endl;
        exit(1);

    }
    return std::make_pair(steps, threads);
}

int main(int argc, const char* argv[]) {

    auto ret_pair = usage(argc, argv);
    auto steps = ret_pair.first;
    auto threads = ret_pair.second;

    my_float pi = 0.0;

    // 1. Vector para guardar los "futuros"
    std::vector<std::future<my_float>> futures;

    size_t chunk_size = steps / threads;
    size_t start = 0;

    // 2. Lanzar las tareas asíncronas
    for (size_t t = 0; t < threads; ++t) {
        size_t stop = (t == threads - 1) ? steps : start + chunk_size;
        // std::launch::async asegura que se ejecute en un hilo nuevo
        futures.emplace_back(std::async(std::launch::async, pi_taylor_chunk, start, stop));
        start = stop;
    }

    // 3. Recoger resultados y reducción
    my_float total_sum = 0.0;
    for (auto& fut : futures) {
        // .get() espera a que la tarea termine y devuelve el valor
        total_sum += fut.get();
    }

    pi = total_sum * 4.0;

    std::cout << "For " << steps << ", pi value: "
        << std::setprecision(std::numeric_limits<long double>::digits10 + 1)
        << pi << std::endl;
}