#include <iostream>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <string>
// �� ����, ����� �� ���
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
//

#define BLOCK_SIZE 20

__global__ void matrix_dot(int* A, int* B, int* C, int size)
{
    int bx = blockIdx.x;
    int by = blockIdx.y;
    int tx = blockIdx.x;
    int ty = blockIdx.y;
    int sum = 0;
    int ia = size * BLOCK_SIZE * by + size * ty;
    int ib = BLOCK_SIZE * bx + tx;
    for (int k = 0; k < size; k++)
        sum += A[ia + k] * B[ib + k * size];
    int ic = size * BLOCK_SIZE * by + BLOCK_SIZE * bx;
    C[ic + size * ty + tx] = sum;
}

class SquareMatrix
{
private:
    int* _data;
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
    SquareMatrix(const SquareMatrix& src)
    {
        *this = src;
    }
    SquareMatrix& operator=(const SquareMatrix& src)
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
    bool operator==(const SquareMatrix& rhs) const
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
    int& operator()(size_t i, size_t j)
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
    double dot(const SquareMatrix& rhs)
    {
        std::chrono::steady_clock::time_point start, end;
        start = std::chrono::steady_clock::now();
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
        end = std::chrono::steady_clock::now();
        return std::chrono::duration <double>(end - start).count();
    }
    double cuda_dot(SquareMatrix& rhs)
    {
        SquareMatrix result = SquareMatrix(_size);
        int* A = _data;
        int* B = rhs._data;
        int* C = result._data;

        int* Adev = NULL;
        int* Bdev = NULL;
        int* Cdev = NULL;

        cudaMalloc((void**)&Adev, _size * _size * sizeof(int));
        cudaMalloc((void**)&Bdev, _size * _size * sizeof(int));
        cudaMalloc((void**)&Cdev, _size * _size * sizeof(int));

        dim3 threads(BLOCK_SIZE, BLOCK_SIZE);
        dim3 blocks(_size / threads.x, _size / threads.y);

        cudaEvent_t begin, end;
        float time = 0;
        cudaEventCreate(&begin);
        cudaEventCreate(&end);

        cudaEventRecord(begin, 0);
        cudaMemcpy(A, Adev, _size * _size * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(B, Bdev, _size * _size * sizeof(int), cudaMemcpyHostToDevice);
        matrix_dot <<<blocks, threads>>> (Adev, Bdev, Cdev, _size);
        cudaMemcpy(C, Cdev, _size * _size * sizeof(int), cudaMemcpyDeviceToHost);
        cudaEventRecord(end, 0);
        cudaEventSynchronize(end);
        cudaEventElapsedTime(&time, begin, end);

        cudaEventDestroy(begin);
        cudaEventDestroy(end);
        cudaFree(Adev);
        cudaFree(Bdev);
        cudaFree(Cdev);
        return time;
    }
    friend std::ostream& operator<<(std::ostream& os, const SquareMatrix& data)
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

int main(int argc, char** argv)
{
    srand(time(0));
    int size = 1000;
    int block_sizes[] = {4, 8, 12, 16, 32, 48, 96 };
    std::string filename;
    for (int i = 0; i < 7; ++i) {
        filename = std::to_string(block_sizes[i]);
        filename += ".txt";
        std::ofstream file(filename);
#ifdef BLOCK_SIZE
#undef BLOCK_SIZE
#define BLOCK_SIZE block_sizes[i]
#endif
        for (int j = 1; j < 11; ++j) {
            SquareMatrix matrix1(j * 96), matrix2(j * 96), matrix3(j * 96);
            for (int k = 0; k < 10; ++k) {
                matrix1.random_fill();
                matrix2.random_fill();
                matrix3 = matrix1;
                file << matrix1.cuda_dot(matrix2) << " ";
                matrix3.dot(matrix2);
                if (!(matrix1 == matrix3)) {
                    std::cout << "Matrices are not equal" << std::endl;
                    return 1;
                }
            }
        }
        file.close();
    }
    return 0;
}