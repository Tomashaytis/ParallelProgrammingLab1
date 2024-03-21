import numpy as np

FIRST_FOLDER = "D:\\Projects\\Visual Studio\\source\\repos\\3 курс\\ParallelProgramming-Lab1\\matrices\\first\\"
SECOND_FOLDER = "D:\\Projects\\Visual Studio\\source\\repos\\3 курс\\ParallelProgramming-Lab1\\matrices\\second\\"
RESULT_FOLDER1 = "D:\\Projects\\Visual Studio\\source\\repos\\3 курс\\ParallelProgramming-Lab1\\matrices\\result1\\"
RESULT_FOLDER2 = "D:\\Projects\\Visual Studio\\source\\repos\\3 курс\\ParallelProgramming-Lab1\\matrices\\result2\\"
MATRIX_COUNT = 100


def load_matrix(filename: str) -> np.ndarray:
    """
    The function loads matrix from file
    :param filename: name of file
    :return: matrix
    """
    with open(filename, 'r') as fp:
        lines = fp.readlines()
    a = []
    for line in lines:
        a.append(list(map(int, line.split())))
    return np.array(a)


def check_matrix_products(matric_count: int, first_folder: str, second_folder: str, result_folder: str) -> None:
    """
    The function checks matrix products for a certain number of matrix
    :param matric_count: number of matrices with the same size
    :param first_folder: folder with first matrices in matrix products
    :param second_folder: folder with second matrices in matrix products
    :param result_folder: folder with results of matrix products
    :return: None
    """
    for k in range(1, 11):
        corrected = 0
        for i in range(matric_count):
            matrix1 = load_matrix(first_folder + f'{k * 100}\\{i}.txt')
            matrix2 = load_matrix(second_folder + f'{k * 100}\\{i}.txt')
            result = load_matrix(result_folder + f'{k * 100}\\{i}.txt')
            true_result = np.dot(matrix1, matrix2)
            if np.array_equal(result, true_result):
                corrected += 1
        print(f'{k * 100}x{k * 100} {corrected}/{matric_count} matrix products corrected')


if __name__ == '__main__':
    print('Check matrix products for OpenMP calculating')
    check_matrix_products(MATRIX_COUNT, FIRST_FOLDER, SECOND_FOLDER, RESULT_FOLDER2)
    print()
    print('Check matrix products for series calculating')
    check_matrix_products(MATRIX_COUNT, FIRST_FOLDER, SECOND_FOLDER, RESULT_FOLDER1)
