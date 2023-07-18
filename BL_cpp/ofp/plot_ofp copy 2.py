import sys
import matplotlib.pyplot as plt
import numpy as np

# Получаем путь к файлу из аргументов командной строки
filename = sys.argv[1]
filename_sw = sys.argv[2]

# Читаем данные из файла
def plot(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    # Разбиваем строки на столбцы и преобразуем в числа
    x = []
    y = []
    z = []
    for line in lines:
        columns = line.strip().split()
        x.append(float(columns[0]))
        y.append(float(columns[1]))
        z.append(float(columns[2]))

    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.plot(x, z)
    ax.figure.savefig(filename)

    x_range = np.linspace(0,1,50)
    ax.clear()
    ax.plot(x, y)
    ax.figure.savefig('sw_'+filename)

def plot2(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    # Разбиваем строки на столбцы и преобразуем в числа
    x = []
    y = []
    
    for line in lines:
        columns = line.strip().split()
        x.append(float(columns[0]))
        y.append(float(columns[1]))
        

    fig, ax = plt.subplots()
    ax.plot(x, y)
    
    ax.figure.savefig(filename)

plot(filename)
plot2(filename_sw)