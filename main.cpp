#include <iostream>
#include <iomanip>
#include "mpi.h"
#include <cstdlib>

int rank, count;

class SquareMatrix
{
private:
    int *_data;
    size_t _size;

public:
    SquareMatrix(size_t initial_size = 10)
    {
        _size = initial_size;
        _data = new int[_size * _size];
    }
    ~SquareMatrix()
    {
        clear();
    }
    SquareMatrix(const SquareMatrix &src)
    {
        *this = src;
    }
    SquareMatrix &operator=(const SquareMatrix &src)
    {
        clear();
        _size = src._size;
        _data = new int[_size * _size];
        for (size_t i = 0; i < _size * _size; i++)
        {
            _data[i] = src._data[i];
        }
        return *this;
    }
    bool operator==(const SquareMatrix &rhs) const
    {
        if (_size != rhs._size)
            return false;
        for (size_t i = 0; i < _size; i++)
        {
            if (_data[i] != rhs._data[i])
                return false;
        }
        return true;
    }
    int &operator()(size_t i, size_t j)
    {
        return _data[i * _size + j];
    }
    int operator()(size_t i, size_t j) const
    {
        return _data[i * _size + j];
    }
    void clear()
    {
        delete[] _data;
        _data = NULL;
        _size = 0;
    }
    size_t size() const
    {
        return _size;
    }
    void random_fill()
    {
        for (size_t i = 0; i < _size * _size; i++)
        {
            int k = rand() % 2;
            (k == 0) ? (k = -1) : (k = 1);
            _data[i] = rand() % 100 * k;
        }
    }
    void fill(int value)
    {
        for (size_t i = 0; i < _size * _size; i++)
        {
            _data[i] = value;
        }
    }
    double dot(const SquareMatrix &rhs)
    {
	double begin = MPI_Wtime();
        SquareMatrix result = SquareMatrix(_size);
        for (size_t i = 0; i < _size; i++)
        {
            for (size_t j = 0; j < _size; j++)
            {
                for (size_t k = 0; k < _size; k++)
                {
                    result(i, j) += (*this)(i, k) * rhs(k, j);
                }
            }
        }
	double end = MPI_Wtime();
        return end - begin;
    }
    double mpi_dot(SquareMatrix &rhs)
    {
        double begin = MPI_Wtime();
        SquareMatrix result = SquareMatrix(_size);
        int *A = _data, *B = rhs._data, *C = result._data;
        MPI_Status status;
        int tmp = 0, ind = 0;
        int part_size = _size / count;
        int part = _size * part_size;
        int *bufA = new int[part];
        int *bufB = new int[part];
        int *bufC = new int[part];
        if (rank == 0)
        {
            for (int i = 0; i < part_size; i++)
            {
                for (int j = 0; j < part_size; j++)
                {
                    tmp = B[i * _size + j];
                    B[i * _size + j] = B[j * _size + i];
                    B[j * _size + i] = tmp;
                }
            }
        }
        MPI_Scatter(A, part, MPI_INT, bufA, part, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(B, part, MPI_INT, bufB, part, MPI_INT, 0, MPI_COMM_WORLD);
        for (int i = 0; i < part_size; i++)
        {
            for (int j = 0; j < part_size; j++)
            {
                tmp = 0;
                for (int k = 0; k < part_size; k++)
                    tmp += bufA[i * _size + k] * bufB[k * _size + j];
                bufC[i * _size + j + part_size * rank] = tmp;
            }
        }
        int next_proc, prev_proc;
        for (int p = 1; p < count; p++)
        {
            next_proc = rank + 1;
            if (rank == count - 1)
                next_proc = 0;
            prev_proc = rank - 1;
            if (rank == 0)
                prev_proc = count - 1;
            MPI_Sendrecv_replace(bufB, part, MPI_INT, next_proc, 0, prev_proc, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < part_size; i++)
            {
                for (int j = 0; j < part_size; j++)
                {
                    tmp = 0;
                    for (int k = 0; k < part_size; k++)
                        tmp += bufA[i * _size + k] * bufB[k * _size + j];
                    if (rank - p >= 0)
                        ind = rank - p;
                    else
                        ind = (count - p + rank);
                    bufC[i * _size + j + ind * part_size] = tmp;
                }
            }
        }
        MPI_Gather(bufC, part, MPI_INT, C, part, MPI_INT, 0, MPI_COMM_WORLD);
        delete[] bufA;
        delete[] bufB;
        delete[] bufC;
        double end = MPI_Wtime();
        *this = result;
        return end - begin;
    }
    friend std::ostream &operator<<(std::ostream &os, const SquareMatrix &data)
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            for (size_t j = 0; j < data.size(); j++)
            {
                os << data(i, j) << '\t';
            }
            os << '\n';
        }
        return os;
    }
};

int main(int argc, char **argv)
{
    srand(time(0));
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &count);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (size_t k = 1; k < 11; k++)
    {
        int size = k * 100;
        SquareMatrix r_matrix(size), l_matrix(size);
        for (size_t i = 0; i < 100; i++)
        {
            r_matrix.random_fill();
            l_matrix.random_fill();
	    double result = r_matrix.mpi_dot(l_matrix);
	    if (rank == 0)
	    	std::cout << result << std::endl;
        }
    }
    MPI_Finalize();
    return 0;
}