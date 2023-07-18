#! /usr/bin/env python3

import sys
import matplotlib.pyplot as plt
import numpy as np
import os
import operator

def plot(filenames):
    fig, axs = plt.subplots(2, 2, figsize=(10, 10))
    for filename in filenames:
        print(f"{filename=}")
        with open(filename, 'r') as f:
            lines = f.readlines()
        std = filename.split("_")[-1]
        std = std.split(".")[:-1]
        std = ".".join([std[0], std[1]])
        
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
        
        
        axs[0, 0].plot(num, mean_no, label = std)
        axs[0, 0].set_title('Mean NO')
        axs[0, 1].plot(num, std_no, label = std)
        axs[0, 1].set_title('Std NO')
        axs[1, 0].plot(num, mean_nw, label = std)
        axs[1, 0].set_title('Mean NW')
        axs[1, 1].plot(num, std_nw, label = std)
        axs[1, 1].set_title('Std NW')

    for ax in axs.flat:
        ax.grid(True, which='major', axis='both', linestyle='--', color='grey', alpha=0.5)
        handles, labels = fig.gca().get_legend_handles_labels()
        label_dict = dict(zip(labels, handles))
        sorted_labels = sorted(label_dict.items(), key=lambda x: x[1].get_ydata()[0])
        sorted_labels = [x[0] for x in sorted_labels]
        ax.legend([label_dict[x] for x in sorted_labels], sorted_labels)
        
        
        
    return fig

def plot_delta(filenames):
    fig, axs = plt.subplots(2, 2, figsize=(10, 10))
    num = []
    mean_no = []
    mean_nw = []
    std_no = []
    std_nw = []
    std_arr = []
    for filename in filenames:
        print(f"{filename=}")
        with open(filename, 'r') as f:
            lines = f.readlines()
        std = filename.split("_")[-1]
        std = std.split(".")[:-1]
        std = ".".join([std[0], std[1]])
        std_arr.append(float(std))
        # Разбиваем строки на столбцы и преобразуем в числа
        
        
        line = lines[-1]
        columns = line.strip().split()
        num.append(int(columns[0]))
        mean_no.append(float(columns[1]))
        std_no.append(float(columns[2]))

        mean_nw.append(float(columns[3]))
        std_nw.append(float(columns[4]))
            

        # Строим график
        
    print(mean_no)
    print(std_arr)

    actual_mean_no = [3.0] * len(mean_no)
    actual_mean_nw = [2.5638] * len(mean_nw)

    axs[0, 0].plot(std_arr, mean_no, label = "Рассчетная")
    axs[0, 0].plot(std_arr, actual_mean_no, label = "Реальная")
    axs[0, 0].set_title('Mean NO')
    axs[0, 1].plot(std_arr, std_no,)
    axs[0, 1].set_title('Std NO')
    axs[1, 0].plot(std_arr, mean_nw, label = "Рассчетная")
    axs[1, 0].plot(std_arr, actual_mean_nw, label = "Реальная")
    axs[1, 0].set_title('Mean NW')
    axs[1, 1].plot(std_arr, std_nw)
    axs[1, 1].set_title('Std NW')
    

    for ax in axs.flat:
        ax.grid(True, which='major', axis='both', linestyle='--', color='grey', alpha=0.5)
        # handles, labels = fig.gca().get_legend_handles_labels()
        # label_dict = dict(zip(labels, handles))
        # sorted_labels = sorted(label_dict.items(), key=lambda x: x[1].get_ydata()[0])
        # sorted_labels = [x[0] for x in sorted_labels]
        # ax.legend([label_dict[x] for x in sorted_labels], sorted_labels)
        ax.legend()
        
        
    fig.suptitle("Мат ожидание и дисперсия от дисперсии шума")  
    return fig



def plot_figs(filenames):
    # Получаем путь к файлу из аргументов командной строки
    save_to = "std_progress.png"

    fig = plot(filenames)
    fig.savefig(save_to)
    print('done ', save_to)

    save_to = "std_progress_300.png"

    fig = plot_delta(filenames)
    fig.savefig(save_to)
    print('done ', save_to)


if __name__ == '__main__':
    path = "./" 
    files = os.listdir(path)
    res_std_files = [f for f in files if f.startswith('res_std') and f.endswith(".txt")]
    print(sorted(res_std_files))
    plot_figs(sorted(res_std_files))

    
    