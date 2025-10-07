#include <iostream>
#include <Eigen/Dense>
#include <ctime>
#include <iomanip>
#include <sys/time.h>

using namespace std;
using Eigen::MatrixXd;
 
int main() {
  struct timeval timestamp;
  gettimeofday(&timestamp, NULL);
  // clock_t time;
  // time = clock();
  int n=2000;
  MatrixXd m1(n, n);
  for (int i = 0 ; i<n;++i){
    for(int j = 0; j<n; ++j){
      m1(i,j)=j+i*2+1;
    }
  }
  MatrixXd m2(n, n);
  for (int i = 0 ; i<n;++i){
    for(int j = 0; j<n; ++j){
      m2(i,j)=j+i*2+5;
    }
  }
long aux_sec = timestamp.tv_sec;
long aux_u = timestamp.tv_usec;
gettimeofday(&timestamp, NULL);
cout << "Inicializar Seconds: " << timestamp.tv_sec - aux_sec << endl
     << "Microseconds: " << timestamp.tv_usec - aux_u << endl;
MatrixXd m = m1 * m2;
// time = clock() - time;
// cout << "seconds: " << fixed << setprecision(4) << ((float)time)/CLOCKS_PER_SEC << endl;
aux_sec = timestamp.tv_sec;
aux_u = timestamp.tv_usec;
gettimeofday(&timestamp, NULL);
cout << "Multiplicar Seconds: " << timestamp.tv_sec - aux_sec << endl
     << "Microseconds: " << timestamp.tv_usec - aux_u << endl;
}