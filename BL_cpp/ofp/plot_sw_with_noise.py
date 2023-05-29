import sys
import matplotlib.pyplot as plt
import numpy as np

def plot(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    # Разбиваем строки на столбцы и преобразуем в числа
    sw = []
    sw_with_noise = []
    for line in lines:
        columns = line.strip().split()
        sw.append(float(columns[0]))
        sw_with_noise.append(float(columns[1]))

    # Строим график
    fig = plt.figure()
    ax = fig.add_subplot(111)
    x = np.linspace(0, 1, len(sw))
    ax.plot(x, sw)
    ax.plot(x, sw_with_noise)
    plt.title("sw(x)")
    plt.xlabel('x')
    plt.ylabel('sw')
    return fig


if __name__=="main":

    # Получаем путь к файлу из аргументов командной строки
    filename = sys.argv[1]
    save_to = sys.argv[2]

    # Читаем данные из файла
    # with open(filename, 'r') as f:
    #     lines = f.readlines()

    # # Разбиваем строки на столбцы и преобразуем в числа
    # sw = []
    # sw_with_noise = []
    # for line in lines:
    #     columns = line.strip().split()
    #     sw.append(float(columns[0]))
    #     sw_with_noise.append(float(columns[1]))

    # # Строим график
    # x = np.linspace(0, 1, len(sw))
    # plt.plot(x, sw)
    # plt.plot(x, sw_with_noise)
    # plt.savefig(save_to)

    fig = plot(filename)
    fig.savefig(save_to)