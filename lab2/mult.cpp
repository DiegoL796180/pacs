#include <iostream>
#include <vector>
#include <sys/time.h>
#include <ctime>
#include <iomanip>


using namespace std;

int main() {
    // clock_t time;
    // time = clock();
    struct timeval timestamp;
    gettimeofday(&timestamp, NULL);
    int n = 3000;
    int const fil = n; //Const porque sino me saltaba error 
    int const col = n;
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
    long aux_sec = timestamp.tv_sec;
    long aux_u = timestamp.tv_usec;
    gettimeofday(&timestamp, NULL);
    cout << "Inicializar Seconds: " << timestamp.tv_sec - aux_sec << endl
        << "Microseconds: " << (timestamp.tv_sec - aux_sec)*1000000L + timestamp.tv_usec - aux_u << endl;
    for (int i = 0; i < fil; ++i) {
        for (int j = 0; j < col; ++j) {
            matrizM[i*col + j] = 0;
            for (int k = 0; k < col; ++k) {
                matrizM[i*col + j] += matriz1[i*col + k] * matriz2[k*col + j];
            }
        }
    }
    // time = clock() - time;
    // cout << "seconds: " << fixed << setprecision(4) << ((float)time)/CLOCKS_PER_SEC << endl;
    aux_sec = timestamp.tv_sec;
    aux_u = timestamp.tv_usec;
    gettimeofday(&timestamp, NULL);
    cout << "Inicializar Seconds: " << timestamp.tv_sec - aux_sec << endl
        << "Microseconds: " << (timestamp.tv_sec - aux_sec)*1000000L + timestamp.tv_usec - aux_u << endl;
    return 0;
}