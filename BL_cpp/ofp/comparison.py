import sys
import matplotlib.pyplot as plt
import numpy as np

# Получаем путь к файлу из аргументов командной строки
filename = sys.argv[1]
filename_rev = sys.argv[2]


def plot2(filename, filename_rev):
    with open(filename, 'r') as f:
        lines = f.readlines()
    with open(filename_rev, 'r') as f:
        lines_rev = f.readlines()

    # Разбиваем строки на столбцы и преобразуем в числа
    x = []
    x_rev = []
    y = []
    y_rev = []
    
    for line in lines:
        columns = line.strip().split()
        x.append(float(columns[0]))
        y.append(float(columns[1]))
    for line in lines_rev:
        columns = line.strip().split()
        x_rev.append(float(columns[0]))
        y_rev.append(float(columns[1]))
        
    mse = mistake(y, y_rev)

    plt.title(f"MSE: {mse}")
    plt.plot(x, y, label="Original")
    plt.plot(x_rev, y_rev, label="Reverse")
    plt.legend()
    
    plt.savefig(f"{filename_rev}.png")
    print("SAVED")

def mistake(sw, sw_rev):
    summary = 0
    counter = 0
    for i in range(len(sw)):
        if sw[i]!=sw[-1] and sw_rev[i] != sw[-1]:
            summary+=np.abs(sw[i]-sw_rev[i])**2
            counter+=1
    summary/=counter
    return summary



plot2(filename, filename_rev)