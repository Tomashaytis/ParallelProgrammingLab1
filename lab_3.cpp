#include "matrix.hpp"

int main(int argc, char** argv) 
{
    srand(time(0));
    MPI_Init(&argc, &argv);
    int rank, count;
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
            double result = r_matrix.mpi_dot(l_matrix, rank, count) << std::endl;
            if (rank == 0)
                std::cout << result;
        }
    }
    MPI_Finalize();
    return 0;
}
