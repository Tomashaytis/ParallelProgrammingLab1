import scipy.stats as sts
import numpy as np
import matplotlib.pyplot as plt
from typing import Tuple, List

STATS_FOLDER1 = "D:\\Projects\\Visual Studio\\source\\repos\\3 курс\\ParallelProgramming-Lab1\\matrices\\statistics1\\"
STATS_FOLDER2 = "D:\\Projects\\Visual Studio\\source\\repos\\3 курс\\ParallelProgramming-Lab1\\matrices\\statistics2\\"
GAMMA = 0.95


def calculate_statistics(statistic_folder: str, gamma: float) -> Tuple[List[float], List[float]]:
    """
    The function calculates statistics for samples of matrix multiplication times
    :param statistic_folder: folder with samples of matrix multiplication times
    :param gamma: gamma value for confidence interval
    :return: list of matrix sizes and list of average time of matrix multiplication for this matrix sizes
    """
    x, y = [], []
    for k in range(1, 11):
        with open(statistic_folder + f'{k * 100}.txt', 'r') as fp:
            sample_t = list(map(float, fp.readlines()))
        print(f'Matrices {k * 100}x{k * 100}:')
        sample_t = sorted(sample_t)
        py_me = np.mean(sample_t)
        py_median = np.median(sample_t)
        py_disp = np.std(sample_t) ** 2
        py_std = np.std(sample_t)
        py_skewness = sts.skew(sample_t)
        py_kurtosis = sts.kurtosis(sample_t)
        print("Mean:", py_me)
        print("Median:", py_median)
        print("Dispersion:", py_disp)
        print("STD:", py_std)
        print("Skewness:", py_skewness)
        print("Kurtosis:", py_kurtosis)
        volume = len(sample_t)
        s = np.sqrt(volume / (volume - 1)) * np.std(sample_t)
        t = sts.t.ppf(df=volume - 1, q=(gamma + 1) / 2)
        delta = t * s / np.sqrt(volume)
        print(f'Confidence interval for GAMMA = {gamma}: ({py_me - delta}, {py_me + delta})')
        print()
        x.append(k * 100)
        y.append(py_me)
    return x, y


if __name__ == '__main__':
    matrix_size1, time1 = calculate_statistics(STATS_FOLDER1, GAMMA)
    matrix_size2, time2 = calculate_statistics(STATS_FOLDER2, GAMMA)
    plt.title('Зависимость времени перемножения матриц от их размера')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(matrix_size1, time1)
    plt.plot(matrix_size2, time2)
    plt.legend(['Последовательное перемножение', 'Технология OpenMP'])
    plt.show()

