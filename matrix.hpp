/*
    Matrix class, with n_row_ number of rows, n_col_ number of columns and  
    several linear algebra functions:
        Matrix addition
        Scalar multiplication
        Matrix multiplication
        Matrix * Vector multiplication
*/
#ifndef MATRIX_
#define MATRIX_
#include "vector.hpp"

template<class T>
class Matrix {
  public:
    Matrix(long long int row, long long int col);
    Matrix(long long int row, long long int col, T initial);
    Matrix(long long int row, long long int col, const Vector<T> &initial);

    T get(unsigned int row, unsigned int col) const;
    T& operator ()(unsigned int row, unsigned int col);
    unsigned int n_row() const;
    unsigned int n_col() const;
    Matrix<T> operator + (const Matrix<T>& other) const;
    Matrix<T> operator += (const Matrix<T>& other);
    Matrix<T> operator * (const Matrix<T>& other) const;
    Matrix<T> operator * (const T &scalar) const;
    Matrix<T> operator *= (const T &scalar);
    
    template<class T2>
    friend Matrix<T2> operator * (const T2 scalar, const Matrix<T2> matrix);
    template<class T3>
    friend Matrix<T3> operator * (const Matrix<T3>& matrix, const Vector<T3>& vector);
  private:
    unsigned int n_row_;
    unsigned int n_col_;
    Vector<Vector<T> > data_;
};

template<class T>
Matrix<T>::Matrix(long long int row, long long int col) {
    if (row < 0 || row > UINT_MAX || col < 0 || col > UINT_MAX) {
        throw std::bad_alloc();
    }
    n_row_ = row;
    n_col_ = col;
    data_.resize(row);
    for (unsigned int i = 0; i < n_row_; i ++) {
        data_[i].resize(n_col_);
    }
}

template<class T>
Matrix<T>::Matrix(long long int row, long long int col, T initial) {
    if (row < 0 || row > UINT_MAX || col < 0 || col > UINT_MAX) {
        throw std::bad_alloc();
    }
    n_row_ = row;
    n_col_ = col;
    data_.resize(row);
    for (unsigned int i = 0; i < n_row_; i ++) {
        data_[i].resize(n_col_);
        for (unsigned int j = 0; j < n_col_; j ++) {
            data_[i][j] = initial;
        }
    }
}

/*
    constructor with Vector
*/
template<class T>
Matrix<T>::Matrix(long long int row, long long int col, const Vector<T> &initial) {
    if (row < 0 || row > UINT_MAX || col < 0 || col > UINT_MAX) {
        throw std::bad_alloc();
    }
    if (initial.size() != row * col) {
        throw std::invalid_argument("Matrix dimension doesn't match its initializing vector");
    }
    n_row_ = row;
    n_col_ = col;
    data_.resize(row);
    for (unsigned int i = 0; i < n_row_; i ++) {
        data_[i].resize(n_col_);
        for (unsigned int j = 0; j < n_col_; j ++) {
            data_[i][j] = initial[i * col + j];
        }
    }
}

template<class T>
T Matrix<T>::get(unsigned int row, unsigned int col) const {
    return data_[row][col];
}

template<class T>
T& Matrix<T>::operator()(unsigned int row, unsigned int col) {
    return data_[row][col];
}

template<class T>
unsigned int Matrix<T>::n_row() const {
    return n_row_;
}

template<class T>
unsigned int Matrix<T>::n_col() const {
    return n_col_;
}

template<class T>
Matrix<T> Matrix<T>::operator + (const Matrix<T>& other) const {
    if (n_row_ != other.n_row_ || n_col_ != other.n_col_) {
        throw std::runtime_error("Matrix dimension must be the same in addition");
    }
    Matrix<T> result(n_row_, n_col_);
    // #pragma omp parallel for
    for (int i = 0 ; i < n_row_; i ++) {
        for (int j = 0 ; j < n_col_; j++){
            result.data_[i][j] = data_[i][j] + other.data_[i][j];
        }
    }
    return result;
}

template<class T>
Matrix<T> Matrix<T>::operator += (const Matrix<T>& other) {
    if (n_row_ != other.n_row_ || n_col_ != other.n_col_) {
        throw std::runtime_error("Matrix dimension must be the same in addition");
    }
    for (int i = 0 ; i < n_row_; i ++) {
        for (int j = 0 ; j < n_col_; j++){
            data_[i][j] = data_[i][j] + other.data_[i][j];
        }
    }
    return *this;
}

/*
    new matrix dimension is (n_row_ , other.n_col_)
    nested loop optimation with 2 x 2 block
    handle edge case of uneven result matrix rows or columns
*/
template<class T>
Matrix<T> Matrix<T>::operator * (const Matrix<T>& other) const {
    if (n_col_ != other.n_row_) {
        throw std::runtime_error("Matrices' dimensions don't match in multiplication");
    }
    const unsigned int col_remainder = other.n_col_ % 2;
    const unsigned int row_remainder = n_row_ % 2;
    Matrix<T> result(n_row_, other.n_col_);
    T tem00, tem01, tem10, tem11;
    for (int i = 0 ; i < (n_row_ - row_remainder); i += 2) {
        for (int j = 0 ; j < (other.n_col_ - col_remainder); j += 2) {
            tem00 = tem01 = tem10 = tem11 = 0;
            for (int h = 0 ; h < n_col_ ; h ++){
                tem00 += other.data_[h][j + 0] * data_[i + 0][h];
                tem01 += other.data_[h][j + 1] * data_[i + 0][h];
                tem10 += other.data_[h][j + 0] * data_[i + 1][h];
                tem11 += other.data_[h][j + 1] * data_[i + 1][h];            
            }
            result.data_[i + 0][j + 0] = tem00;
            result.data_[i + 0][j + 1] = tem01;
            result.data_[i + 1][j + 0] = tem10;
            result.data_[i + 1][j + 1] = tem11;
        }
    }
    if (col_remainder) {
        T tem = 0;
        for (int i = 0 ; i < n_row_ ; i ++){
            tem = 0;
            for (int j = 0 ; j < n_col_ ; j ++){
                tem += data_[i][j] * other.data_[j][other.n_col_ - 1];
            }
            result.data_[i][other.n_col_ - 1] = tem;
        }
    }
    if (row_remainder) {
        T tem = 0;
        for (int i = 0 ; i < other.n_col_ ; i ++){
            tem = 0;
            for (int j = 0 ; j < n_col_ ; j ++){
                tem += data_[n_row_ - 1][j] * other.data_[j][i];
            }
            result.data_[n_row_ - 1][i] = tem;
        }
    }
    return result;
}

template<class T>  
Matrix<T> Matrix<T>::operator * (const T &scalar) const {
    Matrix<T> result(n_row_, n_col_);
    for (unsigned int i = 0 ; i < n_row_ ; i ++){
        for (unsigned int j = 0 ; j < n_col_ ; j ++){
            result.data_[i][j] = data_[i][j] * scalar;
        }
    }
    return result;
}

template<class T>  
Matrix<T> Matrix<T>::operator *= (const T &scalar) {
    for (unsigned int i = 0 ; i < n_row_ ; i ++){
        for (unsigned int j = 0 ; j < n_col_ ; j ++){
            data_[i][j] = data_[i][j] * scalar;
        }
    }
}

/*
    handle scalar * matrix
*/
template<class T>  
Matrix<T> operator * (const T scalar, const Matrix<T> matrix) {
    return matrix * scalar;
}

/*
    matrix * vector
*/
template<class T>  
Matrix<T> operator * (const Matrix<T>& matrix, const Vector<T>& vector) {
    if (matrix.n_col_ != vector.size_) {
        throw std::runtime_error("Matrices' dimensions don't match in multiplication");
    }
    Matrix<T> result(matrix.n_row_, 1);
    T tem = 0;
    for (unsigned int i = 0 ; i < matrix.n_row_ ; i ++){
        tem = 0;
        for (unsigned int j = 0 ; j < matrix.n_col_ ; j ++){
            tem += matrix.data_[i][j] * vector.data_[j];
        }
        result.data_[i][0] = tem;
    }
    return result;
}

#endif // MATRIX_