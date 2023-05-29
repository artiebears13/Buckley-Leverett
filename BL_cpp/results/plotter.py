#! /usr/bin/env python3

import sys
import matplotlib.pyplot as plt
import numpy as np

def plot(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    
    # Разбиваем строки на столбцы и преобразуем в числа
    num = []
    mean_no = []
    mean_nw = []
    std_no = []
    std_nw = []
    
    for line in lines:
        columns = line.strip().split()
        num.append(int(columns[0]))
        mean_no.append(float(columns[1]))
        std_no.append(float(columns[2]))

        mean_nw.append(float(columns[3]))
        std_nw.append(float(columns[4]))
        

    # Строим график
    fig, axs = plt.subplots(2, 2, figsize=(10, 10))
    
    axs[0, 0].plot(num, mean_no)
    axs[0, 0].set_title('Mean NO')
    axs[0, 1].plot(num, std_no, 'tab:orange')
    axs[0, 1].set_title('Std NO')
    axs[1, 0].plot(num, mean_nw, 'tab:green')
    axs[1, 0].set_title('Mean NW')
    axs[1, 1].plot(num, std_nw, 'tab:red')
    axs[1, 1].set_title('Std NW')

    for ax in axs.flat:
        ax.grid(True, which='major', axis='both', linestyle='--', color='grey', alpha=0.5)
        
        
    return fig


if __name__ == '__main__':

    # Получаем путь к файлу из аргументов командной строки
    filename = "res.txt"
    save_to = "res_300_of_300_tmp4_std_1.png"

    fig = plot(filename)
    fig.savefig(save_to)
    print('done')