#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <chrono>
#include "Windows.h"
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
    SquareMatrix& dot(const SquareMatrix& rhs)
    {
        if (rhs._size != _size)
            throw std::invalid_argument("Error! Matrix size was not the same.");
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
        *this = result;
        return *this;
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
    std::string result_folder = std::string(DIRECTORY) + "result\\";
    std::string statistics_folder = std::string(DIRECTORY) + "statistics\\";
    std::chrono::steady_clock::time_point begin, end;
    std::chrono::duration<double> duration;
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
            for (size_t k = 1; k < 11; k++)
            {
                double time = 0;
                std::string size = std::to_string(100 * k) + "\\";
                SquareMatrix matrix1(100 * k), matrix2(100 * k);
                for (size_t i = 0; i < MATRIX_COUNT; i++)
                {
                    matrix1.load(first_folder + size + std::to_string(i) + ".txt");
                    matrix2.load(second_folder + size + std::to_string(i) + ".txt");
                    begin = std::chrono::high_resolution_clock::now();
                    matrix1.dot(matrix2);
                    end = std::chrono::high_resolution_clock::now();
                    duration = end - begin;
                    stats[i] = duration.count();
                    time += duration.count();
                    matrix1.save(result_folder + size + std::to_string(i) + ".txt");
                }
                std::ofstream out;
                out.open(statistics_folder + std::to_string(100 * k) + ".txt");
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
