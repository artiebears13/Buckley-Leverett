import sys
import matplotlib.pyplot as plt

# Получаем путь к файлу из аргументов командной строки
filename = sys.argv[1]

# Читаем данные из файла
with open(filename, 'r') as f:
    lines = f.readlines()

t = []
x = [[]]
sw = [[]]
ko = [[]]
kw = [[]]
pc = [[]]

# for line in lines:
#     columns = line.strip().split()
t0 = lines[2].strip().split()[1]
sw0 = lines[3].strip().split()[1]
swn0 = lines[4].strip().split()[1]
porosity = lines[5].strip().split()[1]
mu_w = lines[6].strip().split()[1]
mu_o = lines[7].strip().split()[1]
h = lines[8].strip().split()[1]
N = int(lines[9].strip().split()[1])
t = lines[10].strip().split()[1:]
lines = lines[11:]
# print(lines[0])
for i in range(len(t)):
    t[i] = float(t[i])
for i in range(len(t)):
    x.append([])
    sw.append([])
    ko.append([])
    kw.append([])
    pc.append([])
    for j in range(N):
        params = lines[i * N + j].strip().split()
        # print(params)
        x[i].append(float(params[0]))
        # print(x[i])
        sw[i].append(float(params[1]))
        ko[i].append(float(params[2]))
        kw[i].append(float(params[3]))
        pc[i].append(float(params[4]))

for i in range(len(t)):
    plt.plot(x[i], sw[i], label=f't = {str(t[i])}')
plt.legend()
plt.savefig(filename+'_SW.png')

rows, cols = (len(t) // 4 + len(t) % 4), 4
fig, axs = plt.subplots(rows, cols, figsize=(20, 7), constrained_layout=True)
if rows == 1:
    axsIndexes = [i for i in range(cols)]
else:
    axsIndexes = [(i, j) for i in range(rows) for j in range(cols)]

plt.suptitle(f"ОФП ", fontsize=20)

for i, time in enumerate(t):
    axs[axsIndexes[i]].plot(sw[i], ko[i], label=f"k_ro", c='green')
    axs[axsIndexes[i]].plot(sw[i], kw[i], label=f"r_rw", c='red')

    axs[axsIndexes[i]].set_title(f"t = {time}")
    axs[axsIndexes[i]].grid(True)
    axs[axsIndexes[i]].legend()
plt.savefig(filename+'_OFP.png')

fig, axs = plt.subplots(rows, cols, figsize=(20, 7), constrained_layout=True)
if rows == 1:
    axsIndexes = [i for i in range(cols)]
else:
    axsIndexes = [(i, j) for i in range(rows) for j in range(cols)]

plt.suptitle(f"Давление ", fontsize=20)

for i, time in enumerate(t):
    axs[axsIndexes[i]].plot(sw[i], pc[i], label=f"k_ro", c='green')
    axs[axsIndexes[i]].set_title(f"t = {time}")
    axs[axsIndexes[i]].grid(True)
    axs[axsIndexes[i]].legend()
plt.savefig(filename+'_PC.png')
print('done!')