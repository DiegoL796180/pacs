#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <chrono>

// Allow to change the floating point type
using my_float = long double;

my_float pi_taylor(size_t steps) {

    my_float pi = 0.0;
    my_float flag = 1.0;
    for (size_t i = 0; i < steps; i++) {
        pi += flag / (2.0 * i + 1.0);
        flag = -flag;
    }
    pi *= 4.0;
    return pi;
}

int main(int argc, const char* argv[]) {

    // read the number of steps from the command line
    if (argc != 2) {
        std::cerr << "Invalid syntax: pi_taylor <steps>" << std::endl;
        exit(1);
    }

    size_t steps = std::stoll(argv[1]);

    // Iniciar la medición del tiempo
    const auto start{ std::chrono::steady_clock::now() };

    auto pi = pi_taylor(steps);

    // Detener la medición del tiempo
    const auto finish{ std::chrono::steady_clock::now() };
    const std::chrono::duration<double> elapsed_seconds{ finish - start };


    std::cout << "For " << steps << ", pi value: "
        << std::setprecision(std::numeric_limits<my_float>::digits10 + 1)
        << pi << std::endl;
    // Imprimir el tiempo transcurrido
    std::cout << "Pi calculated in : " << elapsed_seconds.count() << " seconds" << std::endl;

}