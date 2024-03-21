#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <chrono>
#include "Windows.h"
#include "omp.h"
#include "time.h"
#define DIRECTORY "D:\\Projects\\Visual Studio\\source\\repos\\3 курс\\ParallelProgramming-Lab1\\matrices\\"
#define MATRIX_COUNT 100

class SquareMatrix
{
private:
    int** _data;
    size_t _size;
public:
    SquareMatrix(size_t initial_size = 0)
    {
        _size = initial_size;
        _data = new int*[_size];
        for (size_t i = 0; i < _size; i++)
        {
            _data[i] = new int[_size] {0};
        }
    }
    ~SquareMatrix()
    {
        clear();
    }
    SquareMatrix(const SquareMatrix& src)
    {
        *this = src;
    }
    SquareMatrix& operator =(const SquareMatrix& src)
    {
        clear();
        _size = src._size;
        _data = new int* [_size];
        for (size_t i = 0; i < _size; i++)
        {
            _data[i] = new int[_size];
        }
        for (size_t i = 0; i < _size; i++)
        {
            for (size_t j = 0; j < _size; j++)
            {
                _data[i][j] = src._data[i][j];
            }
        }
        return *this;
    }
    void clear()
    {
        for (size_t i = 0; i < _size; i++)
        {
            delete[] _data[i];
        }
        delete[] _data;
        _data = nullptr;
        _size = 0;
    }
    void random_fill()
    {
        for (size_t i = 0; i < _size; i++)
        {
            for (size_t j = 0; j < _size; j++)
            {
                int k = rand() % 2;
                (k == 0) ? (k = -1) : (k = 1);
                _data[i][j] = rand() % 1000 * k;
            }
        }
    }
    void fill(int value)
    {
        for (size_t i = 0; i < _size; i++)
        {
            for (size_t j = 0; j < _size; j++)
            {
                _data[i][j] = value;
            }
        }
    }
    void print() const
    {
        for (size_t i = 0; i < _size; i++)
        {
            for (size_t j = 0; j < _size; j++)
            {
                std::cout << _data[i][j] << '\t';
            }
            std::cout << '\n';
        }
    }
    void set(size_t i, size_t j, int value)
    {
        if (i >= _size || j >= _size)
            throw std::invalid_argument("Error! Index out of matrix bounds.");
        _data[i][j] = value;
    }
    int get(size_t i, size_t j) const
    {
        if (i >= _size || j >= _size)
            throw std::invalid_argument("Error! Index out of matrix bounds.");
        return _data[i][j];
    }
    double dot(const SquareMatrix& rhs)
    {
        if (rhs._size != _size)
            throw std::invalid_argument("Error! Matrix size was not the same.");
        std::chrono::steady_clock::time_point begin, end;
        std::chrono::duration<double> duration;
        begin = std::chrono::high_resolution_clock::now();
        SquareMatrix result = SquareMatrix(_size);
        for (size_t i = 0; i < _size; i++)
        {
            for (size_t j = 0; j < _size; j++)
            {
                for (size_t k = 0; k < _size; k++)
                {
                    result._data[i][k] += _data[i][j] * rhs._data[j][k];
                }
            }
        }
        end = std::chrono::high_resolution_clock::now();
        duration = end - begin;
        *this = result;
        return duration.count();
    }
    double openmp_dot(const SquareMatrix& rhs)
    {
        if (rhs._size != _size)
            throw std::invalid_argument("Error! Matrix size was not the same.");
        SquareMatrix result = SquareMatrix(_size);
        auto a = _data;
        auto b = rhs._data;
        auto c = result._data;
        double begin = omp_get_wtime();
        int i, j, k;
        #pragma omp parallel for collapse(2) shared(a, b, c) private(i, j, k)
        for (i = 0; i < _size; i++)
        {
            for (j = 0; j < _size; j++)
            {
                for (k = 0; k < _size; k++)
                {
                    c[i][k] += a[i][j] * b[j][k];
                }
            }
        }
        double end = omp_get_wtime();
        *this = result;
        return end - begin;
    }
    void save(std::string filename)
    {
        std::ofstream out;
        out.open(filename);
        if (out.is_open())
        {
            for (size_t i = 0; i < _size; i++)
            {
                for (size_t j = 0; j < _size; j++)
                {
                    out << _data[i][j] << ' ';
                }
                out << '\n';
            }
        }
        out.close();
    }
    void load(std::string filename)
    {
        std::ifstream in;
        in.open(filename);
        if (in.is_open())
        {
            for (size_t i = 0; i < _size; i++)
            {
                for (size_t j = 0; j < _size; j++)
                {
                    in >> _data[i][j];
                }
            }
        }
        in.close();
    }
};

int main()
{
    srand((int)time(0));
    double stats[MATRIX_COUNT];
    std::string first_folder = std::string(DIRECTORY) + "first\\";
    std::string second_folder = std::string(DIRECTORY) + "second\\";
    std::string result1_folder = std::string(DIRECTORY) + "result1\\";
    std::string statistics1_folder = std::string(DIRECTORY) + "statistics1\\";
    std::string result2_folder = std::string(DIRECTORY) + "result2\\";
    std::string statistics2_folder = std::string(DIRECTORY) + "statistics2\\";
    std::string user_input;
    while (true)
    {
        std::cout << "Input command -> ";
        std::cin >> user_input;
        if (user_input == "--exit" || user_input == "-e")
            break;
        else if (user_input == "--generate" || user_input == "-g")
        {
            std::cout << "First matrix:" << std::endl; 
            for (size_t k = 1; k < 11; k++)
            {
                std::string size = std::to_string(100 * k) + "\\";
                SquareMatrix matrix(100 * k);
                for (size_t i = 0; i < MATRIX_COUNT; i++)
                {
                    matrix.random_fill();
                    matrix.save(first_folder + size + std::to_string(i) + ".txt");
                }
                std::cout << "Matrices " + std::to_string(100 * k) + "x" + std::to_string(100 * k) + " - generated (" + std::to_string(MATRIX_COUNT) + ")." << std::endl;
            }
            std::cout << "Second matrix:" << std::endl;
            for (size_t k = 1; k < 11; k++)
            {
                std::string size = std::to_string(100 * k) + "\\";
                SquareMatrix matrix(100 * k);
                for (size_t i = 0; i < MATRIX_COUNT; i++)
                {
                    matrix.random_fill();
                    matrix.save(second_folder + size + std::to_string(i) + ".txt");
                }
                std::cout << "Matrices " + std::to_string(100 * k) + "x" + std::to_string(100 * k) + " - generated (" + std::to_string(MATRIX_COUNT) + ")." << std::endl;
            }
        }
        else if (user_input == "--calculate" || user_input == "-c")
        {
            std::cout << "Series calculating starts\n";
            for (size_t k = 1; k < 11; k++)
            {
                double time = 0;
                std::string size = std::to_string(100 * k) + "\\";
                SquareMatrix matrix1(100 * k), matrix2(100 * k);
                for (size_t i = 0; i < MATRIX_COUNT; i++)
                {
                    matrix1.load(first_folder + size + std::to_string(i) + ".txt");
                    matrix2.load(second_folder + size + std::to_string(i) + ".txt");
                    double wtime = matrix1.dot(matrix2);
                    stats[i] = wtime;
                    time += wtime;
                    matrix1.save(result1_folder + size + std::to_string(i) + ".txt");
                }
                std::ofstream out;
                out.open(statistics1_folder + std::to_string(100 * k) + ".txt");
                if (out.is_open())
                {
                    for (size_t i = 0; i < MATRIX_COUNT; i++)
                    {
                        out << stats[i] << '\n';
                    }
                }
                out.close();
                std::cout << "Matrices " + std::to_string(100 * k) + "x" + std::to_string(100 * k) + " - products calculated (" + std::to_string(time / MATRIX_COUNT) + " s)." << std::endl;
            }
        }
        else if (user_input == "--openmp" || user_input == "-o")
        {
            std::cout << "OpenMP calculating starts\n";
            std::cout << "Available threads: " << omp_get_max_threads() << std::endl;
            for (size_t k = 1; k < 11; k++)
            {
                double time = 0;
                std::string size = std::to_string(100 * k) + "\\";
                SquareMatrix matrix1(100 * k), matrix2(100 * k);
                for (size_t i = 0; i < MATRIX_COUNT; i++)
                {
                    matrix1.load(first_folder + size + std::to_string(i) + ".txt");
                    matrix2.load(second_folder + size + std::to_string(i) + ".txt");
                    double wtime = matrix1.openmp_dot(matrix2);
                    stats[i] = wtime;
                    time += wtime;
                    matrix1.save(result2_folder + size + std::to_string(i) + ".txt");
                }
                std::ofstream out;
                out.open(statistics2_folder + std::to_string(100 * k) + ".txt");
                if (out.is_open())
                {
                    for (size_t i = 0; i < MATRIX_COUNT; i++)
                    {
                        out << stats[i] << '\n';
                    }
                }
                out.close();
                std::cout << "Matrices " + std::to_string(100 * k) + "x" + std::to_string(100 * k) + " - products calculated (" + std::to_string(time / MATRIX_COUNT) + " s)." << std::endl;
            }
        }
        else if (user_input == "--help" || user_input == "-h")
        {
            std::cout << std::endl;
            std::cout << "Available commands:" << std::endl;
            std::cout << "[--exit] or [-e] - exit the programm." << std::endl;
            std::cout << "[--generate] or [-g] - generate matrices." << std::endl;
            std::cout << "[--calculate] or [-c] - calculate matrix progucts." << std::endl;
            std::cout << "[--openmp] or [-o] - calculate matrix progucts by OpenMP techology." << std::endl;
            std::cout << "[--help] or [-h] - help with commands." << std::endl;
            std::cout << std::endl;
        }
        else
        {
            std::cout << "Unrecognaized command " + user_input + ". Input command --help." << std::endl;
        }
    }
    return 0;
}
