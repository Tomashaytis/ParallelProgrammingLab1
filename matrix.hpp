#include "mpi.h"
#include <iostream>
#include <iomanip>
#include <exception>
#include <chrono>

class SquareMatrix
{
private:
    int* _data;
    size_t _size;
public:
    SquareMatrix(size_t initial_size = 10);
    ~SquareMatrix();
    SquareMatrix(const SquareMatrix& src);
    SquareMatrix& operator =(const SquareMatrix& src);
    bool operator ==(const SquareMatrix& rhs) const;
    int& operator()(size_t i, size_t j);
    int operator()(size_t i, size_t j) const;
    void clear();
    size_t size() const;
    void random_fill();
    void fill(int value);
    double dot(const SquareMatrix& rhs);
    double mpi_dot(SquareMatrix& rhs, int rank, int count);
    friend std::ostream& operator <<(std::ostream& os, const SquareMatrix& data);
};