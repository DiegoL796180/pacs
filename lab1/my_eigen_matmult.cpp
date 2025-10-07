#include <iostream>
#include <Eigen/Dense>
 
using Eigen::MatrixXd;
 
int main() {
  int n=10000;
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
}