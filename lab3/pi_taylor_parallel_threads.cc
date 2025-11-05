#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric> // Para std::accumulate
#include <string>
#include <thread>
#include <utility>
#include <vector>

using my_float = long double;

void
pi_taylor_chunk(std::vector<my_float>& output,
    size_t thread_id, size_t start_step, size_t stop_step) {

    // (void)thread_id; // No usamos thread_id, pero está en la firma

    for (size_t i = start_step; i < stop_step; i++) {
        my_float term = 1.0 / (2.0 * i + 1.0);
        if (i % 2 == 0) { // n par
            output[i] = term;
        }
        else { // n impar
            output[i] = -term;
        }
    }
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

    // 1. Crear vector de resultados (pre-alojado) 
    std::vector<my_float> results(steps);

    // 2. Vector para manejar los hilos
    std::vector<std::thread> thread_handles;

    size_t chunk_size = steps / threads;
    size_t start = 0;

    // 3. Lanzar los hilos (Fork)
    for (size_t t = 0; t < threads; ++t) {
        size_t stop = (t == threads - 1) ? steps : start + chunk_size;
        // Lanzamos el hilo. Usamos std::ref para pasar 'results' por referencia
        thread_handles.emplace_back(pi_taylor_chunk, std::ref(results), t, start, stop);
        start = stop;
    }

    // 4. Esperar a que los hilos terminen (Join)
    for (auto& th : thread_handles) {
        th.join();
    }

    // 5. Reducción final 
    my_float sum = std::accumulate(results.begin(), results.end(), (my_float)0.0);
    pi = sum * 4.0;

    std::cout << "For " << steps << ", pi value: "
        << std::setprecision(std::numeric_limits<long double>::digits10 + 1)
        << pi << std::endl;
}