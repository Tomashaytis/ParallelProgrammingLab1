import os.path
import scipy.stats as sts
import numpy as np
import matplotlib.pyplot as plt
from typing import Tuple, List

BLOCK_4 = os.path.join('results', '4.txt')
BLOCK_8 = os.path.join('results', '8.txt')
BLOCK_12 = os.path.join('results', '12.txt')
BLOCK_16 = os.path.join('results', '16.txt')
BLOCK_32 = os.path.join('results', '32.txt')
BLOCK_48 = os.path.join('results', '48.txt')
BLOCK_96 = os.path.join('results', '96.txt')
SEQUENCE = os.path.join('results', 'sequence_production.txt')
GAMMA = 0.95


def calculate_statistics(statistic_file: str, gamma: float, description: str) -> Tuple[List[float], List[float]]:
    """
    The function calculates statistics for samples of matrix multiplication times
    :param description: description for input
    :param statistic_file: file with sample of matrix multiplication times
    :param gamma: gamma value for confidence interval
    :return: list of matrix sizes and list of average time of matrix multiplication for this matrix sizes
    """
    x, y = [], []
    with open(statistic_file, 'r') as fp:
        data = list(map(float, fp.readline().split()))
        if len(data) == 1:
            data = list(map(float, fp.readlines()))
    if description != 'Sequence production':
        for i in range(len(data)):
            data[i] /= 1000
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
            print(description)
            print(f'Matrices {k1 * 96}x{k1 * 96}:')
            print("Mean:", py_me)
            print("Median:", py_median)
            print("Dispersion:", py_disp)
            print("STD:", py_std)
            print("Skewness:", py_skewness)
            print("Kurtosis:", py_kurtosis)
            print(f'Confidence interval for GAMMA = {gamma}: ({py_me - delta}, {py_me + delta})')
            print()
        x.append(k1 * 96)
        y.append(py_me)
    return x, y


if __name__ == '__main__':
    matrix_size4, time4 = calculate_statistics(BLOCK_4, GAMMA, 'Block size: 4')
    matrix_size8, time8 = calculate_statistics(BLOCK_8, GAMMA, 'Block size: 14')
    matrix_size12, time12 = calculate_statistics(BLOCK_12, GAMMA, 'Block size: 12')
    matrix_size16, time16 = calculate_statistics(BLOCK_16, GAMMA, 'Block size: 16')
    matrix_size32, time32 = calculate_statistics(BLOCK_32, GAMMA, 'Block size: 32')
    matrix_size48, time48 = calculate_statistics(BLOCK_48, GAMMA, 'Block size: 48')
    matrix_size96, time96 = calculate_statistics(BLOCK_96, GAMMA, 'Block size: 96')
    matrix_size_s, time_s = calculate_statistics(SEQUENCE, GAMMA, 'Sequence production')

    fig1 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц от их размера')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(matrix_size4, time4)
    plt.plot(matrix_size8, time8)
    plt.plot(matrix_size12, time12)
    plt.plot(matrix_size16, time16)
    plt.plot(matrix_size32, time32)
    plt.plot(matrix_size48, time48)
    plt.plot(matrix_size96, time96)
    plt.legend(['Блок 4x4', 'Блок 8x8', 'Блок 12x12', 'Блок 16x16', 'Блок 32x32', 'Блок 48x48', 'Блок 96x96'])
    plt.show()

    fig2 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц от их размера')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(matrix_size4, time4)
    plt.plot(matrix_size8, time8)
    plt.plot(matrix_size12, time12)
    plt.plot(matrix_size16, time16)
    plt.plot(matrix_size32, time32)
    plt.plot(matrix_size48, time48)
    plt.plot(matrix_size96, time96)
    plt.semilogy()
    plt.legend(['Блок 4x4', 'Блок 8x8', 'Блок 12x12', 'Блок 16x16', 'Блок 32x32', 'Блок 48x48', 'Блок 96x96'])
    plt.show()

    fig3 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц от их размера')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(matrix_size_s, time_s)
    plt.plot(matrix_size4, time4)
    plt.plot(matrix_size8, time8)
    plt.plot(matrix_size12, time12)
    plt.plot(matrix_size16, time16)
    plt.plot(matrix_size32, time32)
    plt.plot(matrix_size48, time48)
    plt.plot(matrix_size96, time96)
    plt.legend(['Последовательное перемножение', 'Блок 4x4', 'Блок 8x8', 'Блок 12x12', 'Блок 16x16', 'Блок 32x32',
                'Блок 48x48', 'Блок 96x96'])
    plt.show()

    fig4 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц от их размера')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(matrix_size_s, time_s)
    plt.plot(matrix_size4, time4)
    plt.plot(matrix_size8, time8)
    plt.plot(matrix_size12, time12)
    plt.plot(matrix_size16, time16)
    plt.plot(matrix_size32, time32)
    plt.plot(matrix_size48, time48)
    plt.plot(matrix_size96, time96)
    plt.semilogy()
    plt.legend(['Последовательное перемножение', 'Блок 4x4', 'Блок 8x8', 'Блок 12x12', 'Блок 16x16', 'Блок 32x32',
                'Блок 48x48', 'Блок 96x96'])
    plt.show()

    time_per_block = [time4[-1], time8[-1], time12[-1], time16[-1], time32[-1], time48[-1], time96[-1]]
    block_size = [4, 8, 12, 16, 32, 48, 96]
    fig5 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц 960x960 от размера блока')
    plt.xlabel('Размер блока')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(block_size, time_per_block)
    plt.show()

    time_per_block = [time4[0], time8[0], time12[0], time16[0], time32[0], time48[0], time96[0]]
    block_size = [4, 8, 12, 16, 32, 48, 96]
    fig6 = plt.figure(figsize=(10, 6))
    plt.title('Зависимость времени перемножения матриц 96x96 от размера блока')
    plt.xlabel('Размер блока')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(block_size, time_per_block)
    plt.show()
