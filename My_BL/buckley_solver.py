import numpy as np
import matplotlib.pyplot as plt

'''
phi - porosity
'''


class BLproblem:
    phi = 0.2
    kw0 = 0.9
    ko0 = 1
    nw = 2
    no = 1
    m = 2
    mu_w = 0.7e-3
    mu_o = 1.e-3
    u = 3  # inject rate
    lenght = 1

    def __init__(self, Nx: int, sw0: float, so0: float):
        self.N = Nx
        self.sw_prev = np.ones(Nx) * so0
        self.sw_prev[0] = sw0
        self.so_prev = np.ones(Nx) * so0
        # self.tau = (tn - t0) / Nt
        self.sw = np.zeros(Nx)
        self.so = np.zeros(Nx)
        # self.t0 = t0
        # self.tn = tn
        self.h = self.lenght / (Nx - 1)

    def k_rw(self, sw):
        return (sw ** self.nw) * self.kw0

    def k_ro(self, sw):
        return ((1 - sw) ** self.no) * self.ko0

    def pc(self,sw):
        return sw**self.m

    def sigma(self, sw):
        return sw * self.phi

    def f(self, sw):
        up = self.k_rw(sw) / self.mu_w
        down = self.k_rw(sw) / self.mu_w + self.k_ro(sw) / self.mu_o
        return up / down

    def g(self, sw):
        return self.u * self.f(sw)

    def solver(self, t0, tn, steps, sw0, swn):
        t = t0
        tau = (tn - t0) / steps
        print('tau: ',tau)
        self.sw[0] = sw0
        self.sw[-1] = swn
        while t <= tn:
            # print(t)
            self.sw[0] = sw0
            self.sw[-1] = swn
            for i in range(1, len(self.sw) - 1):
                self.sw[i] = tau / self.phi * (self.g(self.sw_prev[i]) - self.g(self.sw_prev[i - 1])) / self.h + \
                             self.sw_prev[i]
                # print((self.g(self.sw_prev[i]) - self.g(self.sw_prev[i - 1])) / self.h)
            t += tau
            # print(self.sw)
            self.sw_prev = self.sw
        x = np.arange(0, 1 + self.h / 2, self.h)
        # plt.plot(x, self.sw)
        # plt.show()
        return self.sw, x

    def plot_OFP(self, sw):
        k_w = np.zeros(len(sw))
        k_o = np.zeros(len(sw))
        for i in range(len(sw)):
            k_w[i] = self.k_rw(sw[i])
            k_o[i] = self.k_ro(sw[i])
        return k_w, k_o

    def plot_PC(self,sw):
        p = np.zeros(len(sw))
        for i in range(len(sw)):
            p[i] = self.pc(sw[i])
        return p



sw0 = 80
so0 = 20
A = BLproblem(100, sw0, so0)

res_u = []
res_x = []
times = [1, 10, 20, 40, 50, 100, 200]
# times = [200]
for time in times:
    u, x = A.solver(0, time, 100*time, sw0, so0)
    res_u.append(u/100)
    res_x.append(x)
    plt.plot(x, u, label=str(time))

plt.legend()
plt.show()
plt.close()

k_w, k_o = A.plot_OFP(res_u[0])
plt.plot(res_u[0], k_w, label = 'k_rw')
plt.plot(res_u[0], k_o, label = 'k_ro')
plt.legend()
plt.show()
plt.close()

p = A.plot_PC(res_u[0])
plt.plot(res_u[0], p)
plt.show()