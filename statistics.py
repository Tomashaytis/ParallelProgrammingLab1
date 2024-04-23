import os.path
import scipy.stats as sts
import numpy as np
import matplotlib.pyplot as plt
from typing import Tuple, List

PROC_16 = os.path.join('results', '16-proc.out')
PROC_14 = os.path.join('results', '14-proc.out')
PROC_12 = os.path.join('results', '12-proc.out')
PROC_10 = os.path.join('results', '10-proc.out')
PROC_8 = os.path.join('results', '8-proc.out')
PROC_6 = os.path.join('results', '6-proc.out')
PROC_4 = os.path.join('results', '4-proc.out')
PROC_2 = os.path.join('results', '2-proc.out')
PROC_1 = os.path.join('results', '1-proc.out')
GAMMA = 0.95


def calculate_statistics(statistic_file: str, gamma: float, n_proc: int) -> Tuple[List[float], List[float]]:
    """
    The function calculates statistics for samples of matrix multiplication times
    :param n_proc: number of available processes
    :param statistic_file: file with sample of matrix multiplication times
    :param gamma: gamma value for confidence interval
    :return: list of matrix sizes and list of average time of matrix multiplication for this matrix sizes
    """
    x, y = [], []
    with open(statistic_file, 'r') as fp:
        data = list(map(float, fp.readlines()))
    size = len(data) // 10
    for k in range(10):
        k1 = k + 1
        sample_t = data[k * size:(k + 1) * size]
        sample_t = sorted(sample_t)
        py_me = np.mean(sample_t)
        py_median = np.median(sample_t)
        py_disp = np.std(sample_t) ** 2
        py_std = np.std(sample_t)
        py_skewness = sts.skew(sample_t)
        py_kurtosis = sts.kurtosis(sample_t)
        volume = len(sample_t)
        s = np.sqrt(volume / (volume - 1)) * np.std(sample_t)
        t = sts.t.ppf(df=volume - 1, q=(gamma + 1) / 2)
        delta = t * s / np.sqrt(volume)
        if k == 9:
            print('Number of process: ', n_proc)
            print(f'Matrices {k1 * 100}x{k1 * 100}:')
            print("Mean:", py_me)
            print("Median:", py_median)
            print("Dispersion:", py_disp)
            print("STD:", py_std)
            print("Skewness:", py_skewness)
            print("Kurtosis:", py_kurtosis)
            print(f'Confidence interval for GAMMA = {gamma}: ({py_me - delta}, {py_me + delta})')
            print()
        x.append(k1 * 100)
        y.append(py_me)
    return x, y


if __name__ == '__main__':
    matrix_size16, time16 = calculate_statistics(PROC_16, GAMMA, 16)
    matrix_size14, time14 = calculate_statistics(PROC_14, GAMMA, 14)
    matrix_size12, time12 = calculate_statistics(PROC_12, GAMMA, 12)
    matrix_size10, time10 = calculate_statistics(PROC_10, GAMMA, 10)
    matrix_size8, time8 = calculate_statistics(PROC_8, GAMMA, 8)
    matrix_size6, time6 = calculate_statistics(PROC_6, GAMMA, 6)
    matrix_size4, time4 = calculate_statistics(PROC_4, GAMMA, 4)
    matrix_size2, time2 = calculate_statistics(PROC_2, GAMMA, 2)
    matrix_size1, time1 = calculate_statistics(PROC_1, GAMMA, 1)
    fig1 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц от их размера')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(matrix_size16, time16)
    plt.plot(matrix_size14, time14)
    plt.plot(matrix_size12, time12)
    plt.plot(matrix_size10, time10)
    plt.plot(matrix_size8, time8)
    plt.plot(matrix_size6, time6)
    plt.plot(matrix_size4, time4)
    plt.plot(matrix_size2, time2)
    plt.plot(matrix_size1, time1)
    plt.legend(['16 ядер', '14 ядер', '12 ядер', '10 ядер', '8 ядер', '6 ядер', '4 ядра', '2 ядра', '1 ядро'])
    plt.show()

    fig2 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц от их размера')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(matrix_size16, time16)
    plt.plot(matrix_size14, time14)
    plt.plot(matrix_size12, time12)
    plt.plot(matrix_size10, time10)
    plt.plot(matrix_size8, time8)
    plt.plot(matrix_size6, time6)
    plt.plot(matrix_size4, time4)
    plt.plot(matrix_size2, time2)
    plt.plot(matrix_size1, time1)
    plt.semilogy()
    plt.legend(['16 ядер', '14 ядер', '12 ядер', '10 ядер', '8 ядер', '6 ядер', '4 ядра', '2 ядра', '1 ядро'])
    plt.show()

    time_n_procs = [time1[-1], time2[-1], time4[-1], time6[-1], time8[-1],
                    time10[-1], time12[-1], time14[-1], time16[-1]]
    n_procs = [1, 2, 4, 6, 8, 10, 12, 14, 16]
    fig3 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц 1000x1000 от количества ядер')
    plt.xlabel('Количество ядер')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(n_procs, time_n_procs)
    plt.show()

    fig4 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц 1000x1000 от количества ядер')
    plt.xlabel('Количество ядер')
    plt.ylabel('Время перемножения матриц, с')
    plt.semilogy()
    plt.plot(n_procs, time_n_procs)
    plt.show()

    time_n_procs = [time1[0], time2[0], time4[0], time6[0], time8[0],
                    time10[0], time12[0], time14[0], time16[0]]
    n_procs = [1, 2, 4, 6, 8, 10, 12, 14, 16]
    fig5 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц 100x100 от количества ядер')
    plt.xlabel('Количество ядер')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(n_procs, time_n_procs)
    plt.show()

    fig6 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц 100x100 от количества ядер')
    plt.xlabel('Количество ядер')
    plt.ylabel('Время перемножения матриц, с')
    plt.semilogy()
    plt.plot(n_procs, time_n_procs)
    plt.show()
