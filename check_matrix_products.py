import numpy as np

FIRST_FOLDER = "D:\\Projects\\Visual Studio\\source\\repos\\3 курс\\ParallelProgramming-Lab1\\matrices\\first\\"
SECOND_FOLDER = "D:\\Projects\\Visual Studio\\source\\repos\\3 курс\\ParallelProgramming-Lab1\\matrices\\second\\"
RESULT_FOLDER = "D:\\Projects\\Visual Studio\\source\\repos\\3 курс\\ParallelProgramming-Lab1\\matrices\\result\\"
MATRIX_COUNT = 100


def load_matrix(filename: str) -> np.ndarray:
    with open(filename, 'r') as fp:
        lines = fp.readlines()
    a = []
    for line in lines:
        a.append(list(map(int, line.split())))
    return np.array(a)


if __name__ == '__main__':
    for k in range(1, 11):
        corrected = 0
        for i in range(MATRIX_COUNT):
            matrix1 = load_matrix(FIRST_FOLDER + f'{k * 100}\\{i}.txt')
            matrix2 = load_matrix(SECOND_FOLDER + f'{k * 100}\\{i}.txt')
            result = load_matrix(RESULT_FOLDER + f'{k * 100}\\{i}.txt')
            true_result = np.dot(matrix1, matrix2)
            if np.array_equal(result, true_result):
                corrected += 1
        print(f'{k * 100}x{k * 100} {corrected}/{MATRIX_COUNT} matrix products corrected')
