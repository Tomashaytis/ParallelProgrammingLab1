import scipy.stats as sts
import numpy as np
import matplotlib.pyplot as plt

STATS_FOLDER = "D:\\Projects\\Visual Studio\\source\\repos\\3 курс\\ParallelProgramming-Lab1\\matrices\\statistics\\"
GAMMA = 0.95

if __name__ == '__main__':
    x, y = [], []
    for k in range(1, 11):
        with open(STATS_FOLDER + f'{k * 100}.txt', 'r') as fp:
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
        t = sts.t.ppf(df=volume - 1, q=(GAMMA + 1) / 2)
        delta = t * s / np.sqrt(volume)
        print(f'Confidence interval for GAMMA = {GAMMA}: ({py_me - delta}, {py_me + delta})')
        print()
        x.append(k * 100)
        y.append(py_me)
    plt.title('Зависимость времени перемножения матриц от их размера')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время перемножения матриц, с')
    plt.plot(x, y)
    plt.show()

