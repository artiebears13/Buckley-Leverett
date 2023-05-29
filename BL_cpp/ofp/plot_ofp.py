import sys
import matplotlib.pyplot as plt
import numpy as np

# Получаем путь к файлу из аргументов командной строки
filename = sys.argv[1]

# Читаем данные из файла
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

# Строим график
plt.plot(x, y)
plt.plot(x, z)
plt.savefig(filename)

x_range = np.linspace(0,1,50)
plt.clear()
plt.plot(x, y)
plt.savefig('sw_'+filename)