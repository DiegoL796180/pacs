#include <iostream>
#include <Eigen/Dense>
#include <ctime>
#include <iomanip>

using namespace std;
using Eigen::MatrixXd;
 
int main() {
  clock_t time;
  time = clock();
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
MatrixXd m = m1 * m2;
time = clock() - time;
cout << "seconds: " << fixed << setprecision(4) << ((float)time)/CLOCKS_PER_SEC << endl;
}