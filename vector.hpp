/*
    Column vector class, with several linear algebra functions:
        Vector addition
        Scalar multiplication
*/
#ifndef VECTOR_
#define VECTOR_

#include <algorithm>
#include <climits>
#include <math.h>
#include <new>

template<class T>
class Matrix;

template <class T>
class Vector {
  public:
    typedef T* iterator;
    Vector();
    Vector(unsigned long long size);
    Vector(unsigned long long size, const T & initial);
    Vector(unsigned long long size, T* array);
    Vector(const Vector<T>& other);
    // move constructor
    Vector(Vector&& other);
    ~Vector();
    
    // copy assignment
    Vector<T>& operator = (const Vector<T> & other);
    // move assignment
    Vector<T>& operator = (Vector<T> && other);

    Vector<T> operator + (const Vector<T> & other) const;
    Vector<T>& operator += (const Vector<T> & other);
    Vector<T> operator * (const T &scalar) const;
    Vector<T> operator *= (const T &scalar);
    bool operator == (const Vector<T> & other) const;
    unsigned int capacity() const;
    unsigned int size() const;
    iterator begin();
    iterator end();
    T& front();
    T& back();
    void push_back(const T& value);
    void reserve(const unsigned int capacity);
    void resize(const unsigned int size);
    T& operator[](unsigned int index) const;
    T& at(unsigned int index) const;
    void clear();

    template<class T2>
    friend Vector<T2> operator * (const T2 scalar, const Vector<T2> vector);
    template<class T3>
    friend Matrix<T3> operator * (const Matrix<T3>& matrix, const Vector<T3>& vector);
  private:
    unsigned int size_;
    unsigned int capacity_;
    T* data_;
};

template<class T>
Vector<T>::Vector(): capacity_(0), size_(0) {
    data_ = NULL;
}

template<class T>
Vector<T>::Vector(unsigned long long size): size_(size), capacity_(size){
    if (size < 0 || size > UINT_MAX) {
        throw std::bad_alloc();
    }
    data_ = new T[capacity_];
}

template<class T>
Vector<T>::Vector(unsigned long long size, const T& initial): size_(size), capacity_(size) {
    if (size < 0 || size > UINT_MAX) {
        throw std::bad_alloc();
    }
    data_ = new T [capacity_];
    for (unsigned int i = 0; i < size; i++) {
        data_[i] = initial;
    }
}

template<class T>
Vector<T>::Vector(unsigned long long size, T* initial): size_(size), capacity_(size) {
    if (size < 0 || size > UINT_MAX) {
        throw std::bad_alloc();
    }
    data_ = new T [capacity_];
    for (unsigned int i = 0; i < size; i++) {
        data_[i] = initial[i];
    }    
}

template<class T>
Vector<T>::Vector(const Vector<T>& other): size_(other.size()), capacity_(other.capacity_) {
    data_ = new T[capacity_];
    for (unsigned int i = 0; i < size_; i++) {
        data_[i] = other.data_[i];
    }
}

template<class T>
Vector<T>::Vector(Vector<T>&& other): size_(other.size()), capacity_(other.capacity_) {
    data_ = other.data_;
    other.data_ = NULL;
}

template<class T>
Vector<T>::~Vector() {
    delete[] data_;
}

template<class T>
Vector<T>& Vector<T>::operator = (const Vector<T>& other) {
    if (this != &other) {
        delete[] data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = new T [capacity_];
        for (unsigned int i = 0; i < size_; i++) {
            data_[i] = other.data_[i];
        }
    }
    return *this;
}

template<class T>
Vector<T> & Vector<T>::operator = (Vector<T>&& other) {
    if (this != &other) {
        delete[] data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = other.data_;
        other.data_ = NULL;
    }
    return *this;
}

template<class T>
Vector<T> Vector<T>::operator + (const Vector<T>& other) const {
    if (size_ != other.size_) {
        throw std::runtime_error("Addition of difference size of vectors is not supported") ;
    }
    Vector<T> sum(size_);
    for (unsigned int i = 0; i < size_; i ++) {
        sum[i] = data_[i] + other[i];
    }
    return sum;
}

template<class T>
Vector<T>& Vector<T>::operator += (const Vector<T>& other) {
    if (size_ != other.size_) {
        throw std::runtime_error("Addition of different size of vectors is not supported") ;
    }
    for (unsigned int i = 0; i < size_; i ++) {
        data_[i] = data_[i] + other[i];
    }
    return *this;
}

template<class T>
Vector<T> Vector<T>::operator * (const T & scalar) const {
    Vector<T> multipli(size_);
    for (unsigned int i = 0; i < size_; i ++) {
        multipli[i] = data_[i] * scalar;
    }
    return multipli;
}

template<class T>
Vector<T> Vector<T>::operator *= (const T & scalar) {
    for (unsigned int i = 0; i < size_; i ++) {
        data_[i] = data_[i] * scalar;
    }
    return *this;
}

template<class T>
bool Vector<T>::operator == (const Vector<T>& other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (unsigned int i = 0; i < size_; i ++) {
        if (data_[i] != other[i]) {
            return false;
        }
    }
    return true;
}

template<class T>
typename Vector<T>::iterator Vector<T>::begin() {
    return data_;
}

template<class T>
typename Vector<T>::iterator Vector<T>::end() {
    return data_ + size();
}

template<class T>
T& Vector<T>::front() {
    return data_[0];
}

template<class T>
T& Vector<T>::back() {
    return data_[size_ - 1];
}

template<class T>
void Vector<T>::push_back(const T & v) {
    if (size_ == capacity_) {
        // std::max to handle capacity_ = 0
        reserve(std::max(2 * capacity_, (unsigned int)1));
    }
    data_[size_] = v;
    size_ ++;
}

template<class T>
void Vector<T>::reserve(const unsigned int capacity) {
    if (capacity > capacity_) {
        T * new_data = new T[capacity];
        for (unsigned int i = 0; i < size_; i++) {
            new_data[i] = data_[i];
        }
        capacity_ = capacity;
        delete[] data_;
        data_ = new_data;
    }
}

template<class T>
unsigned int Vector<T>::size() const {
    return size_;
}

/*
    If size is smaller than the current container size, 
    the content is reduced to its first size-th elements
    removing those beyond (and destroying them
*/ 
template<class T>
void Vector<T>::resize(const unsigned int size) { 
    if (size < size_) {
        capacity_ = size;
        T* new_data = new T [size];
        for (unsigned int i = 0; i < size; i++) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
    }
    else if (size > capacity_) {
        reserve(size);
    }
    size_ = size;
}

/*
    Don't offer bound check with [], similiar to std::vector
*/
template<class T>
T& Vector<T>::operator[](unsigned int index) const {
    return data_[index];
}

/*
    Offer bound check with at(), similiar to std::vector
*/
template<class T>
T& Vector<T>::at(unsigned int index) const {
    if (index > (size_ - 1)) {
        throw std::runtime_error("vector index out of bounds") ;
    }
    return data_[index];
}

template<class T>
unsigned int Vector<T>::capacity() const {
    return capacity_;
}

template <class T>
void Vector<T>::clear() {
    delete[] data_;
    capacity_ = 0;
    size_ = 0;
}

/*
    handle scalar * vector
*/
template <class T>
Vector<T> operator * (const T scalar, const Vector<T> vector) {
    return vector * scalar;
}
 
#endif // VECTOR_
