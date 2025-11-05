#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <future> // Necesario para std::async y std::future

//  Cambiamos a float para esta prueba
using my_float = float;

my_float
pi_taylor_chunk(size_t start_step, size_t stop_step) {

    my_float local_sum = 0.0f;
    my_float c = 0.0f; // Compensaci�n de Kahan

    for (size_t i = start_step; i < stop_step; i++) {
        my_float term = 1.0f / (2.0f * i + 1.0f);
        if (i % 2 != 0) { // n impar
            term = -term;
        }

        // Algoritmo de suma de Kahan 
        my_float y = term - c;
        my_float t = local_sum + y;
        c = (t - local_sum) - y;
        local_sum = t;
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

    my_float pi;

    std::vector<std::future<my_float>> futures;
    size_t chunk_size = steps / threads;
    size_t start = 0;

    for (size_t t = 0; t < threads; ++t) {
        size_t stop = (t == threads - 1) ? steps : start + chunk_size;
        futures.emplace_back(std::async(std::launch::async, pi_taylor_chunk, start, stop));
        start = stop;
    }

    // Reducción final usando suma de Kahan 
    my_float total_sum = 0.0f;
    my_float c = 0.0f; // Compensación de Kahan para la reducción final

    for (auto& fut : futures) {
        my_float partial_sum = fut.get(); // Obtenemos la suma parcial del hilo

        my_float y = partial_sum - c;
        my_float t = total_sum + y;
        c = (t - total_sum) - y;
        total_sum = t;
    }

    pi = total_sum * 4.0f;


    std::cout << "For " << steps << ", pi value: "
        // Ajustamos la precisión para 'float'
        << std::setprecision(std::numeric_limits<float>::digits10 + 1)
        << pi << std::endl;
}