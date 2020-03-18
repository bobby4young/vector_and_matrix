#include <iostream>
#include <climits>
#include <chrono> 

#include "vector.hpp"
#include "matrix.hpp"

template <class T>
void printVector(const Vector<T> v) {
  for (unsigned int i = 0; i < v.size(); i ++) {
    std::cout << v[i] << " ";
  }
  std::cout << std::endl;
}

template <class T>
void printMatrix(const Matrix<T> m) {
  for (unsigned int i = 0; i < m.n_row(); i ++) {
    for (unsigned int j = 0; j < m.n_col(); j ++) {
      std::cout << m.get(i, j) << " ";
    }
    std::cout << std::endl;
  }
}

int main() {
  /* 
    Vector addition
  */
  Vector<int> v1(10, 1);
  Vector<int> v2(10);
  for (unsigned int i = 0; i < v2.size(); i ++) {
    v2[i] = i;
  }
  Vector<int> v3 = v1 + v2;
  std::cout << "Vector addition " << std::endl;
  printVector(v1);
  std::cout << "+ " << std::endl;
  printVector(v2);
  std::cout << "= " << std::endl;
  printVector(v3);
  std::cout << std::endl;

  /* 
    Matrix addition
  */
  std::cout << "Matrix addition " << std::endl;
  Matrix<double> m1(3, 3, 0.1);
  m1(1, 1) = 1.1;
  Matrix<double> m2(3, 3, 0.9);
  Matrix<double> m3 = m1 + m2;
  printMatrix(m1);
  std::cout << "+ " << std::endl;
  printMatrix(m2);
  std::cout << "= " << std::endl;
  printMatrix(m3);
  std::cout << std::endl;
  
  /* 
    Vector scalar multiplication
  */
  std::cout << "Vector scalar multiplication " << std::endl;
  Vector<int> v4 = 6 * v1;
  std::cout << "6 * " << std::endl;
  printVector(v1);
  std::cout << "= " << std::endl;
  printVector(v4);
  std::cout << std::endl;
  
  /* 
    Matrix scalar multiplication
  */
  std::cout << "Matrix scalar multiplication " << std::endl;
  Matrix<double> m4 = 0.5 * m1;
  std::cout << "0.5 * " << std::endl;
  printMatrix(m1);
  std::cout << "= " << std::endl;
  printMatrix(m4);
  std::cout << std::endl;

  /* 
    Matrix multiplication
  */
  std::cout << "Matrix multiplication " << std::endl;
  int array1[] = {1, 0, 0, 1};
  Vector<int> v5(4, array1);
  Matrix<int> identity_ma(2, 2, v5);
  Matrix<int> m5(2, 2, 3);
  Matrix<int> m6 = m5 * identity_ma;
  Matrix<int> m7 = identity_ma * m5;
  printMatrix(m5);
  std::cout << "* " << std::endl;
  printMatrix(identity_ma);
  std::cout << "= " << std::endl;
  printMatrix(m6);
  std::cout << "should be the same as " << std::endl;
  printMatrix(m7);
  std::cout << std::endl;

  /* 
    Matrix * Vector
  */
  std::cout << "Matrix * Vector " << std::endl;
  int array2[] = {2, 2};
  Vector<int> v6(2, array2);
  Matrix<int> m8 = identity_ma * v6;
  printMatrix(identity_ma);
  std::cout << "* " << std::endl;
  printVector(v6);
  std::cout << "= " << std::endl;
  printMatrix(m8);
  std::cout << std::endl;
}