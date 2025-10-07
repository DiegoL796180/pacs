#include <iostream>
#include <vector>
using namespace std;

int main() {
    int const fil = 5000; //Const porque sino me saltaba error 
    int const col = 5000;
    std::vector<double> matriz1(fil*col);
    std::vector<double> matriz2(fil*col);
    std::vector<double> matrizM(fil*col);
    // Inicializar matriz1
    for (int i = 0; i < fil; ++i) {
        for (int j = 0; j < col; ++j) {
            matriz1[i*col + j] = j + i*2 + 1;
        }
    }

    // Inicializar matriz2
    for (int i = 0; i < fil; ++i) {
        for (int j = 0; j < col; ++j) {
            matriz2[i*col + j] = j + i*2 + 5;
        }
    }

    for (int i = 0; i < fil; ++i) {
        for (int j = 0; j < col; ++j) {
            matrizM[i*col + j] = 0;
            for (int k = 0; k < col; ++k) {
                matrizM[i*col + j] += matriz1[i*col + k] * matriz2[k*col + j];
            }
        }
    }
    return 0;
}