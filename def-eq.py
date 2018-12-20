"""
solves def eg y'' - e^-y = 0 ; y(0) =0;  y(1) = b
"""
import math
from threading import Thread, Barrier
from datetime import datetime, timedelta

import matplotlib.pyplot as plt
import numpy as np


class ThreeDiagonalEq:
    """
    [[a b 0 0 ... 0]
     [c a b 0 ... 0]
     [0 c a b ... 0]
     ...
     [0 ... 0 c a b]
     [0 ... 0 0 c a]]

     = [f ... f]^T
    """

    def __init__(self, size: int):
        """
        :param size: matrix size
        """
        self.size = size
        self.A = np.zeros(size)
        self.B = np.zeros(size - 1)
        self.C = np.zeros(size - 1)
        self.F = np.zeros(size)

    def solve(self):
        A1 = self.A.copy()
        F1 = self.F.copy()

        for i in range(1, self.size):
            A1[i] -= self.C[i - 1] * self.B[i - 1] / A1[i - 1]
            F1[i] -= self.C[i - 1] * F1[i - 1] / A1[i - 1]
        F1[-1] /= A1[-1]
        for i in range(2, self.size + 1):
            F1[-i] = (F1[-i] - self.B[-i + 1] * F1[-i + 1]) / A1[-i]
        return F1


    def reduction1step(self):
        if (self.size - 1) % 2 != 0:
            raise RuntimeError(f'{self.size} can\'t be reducted')
        eq = ThreeDiagonalEq(self.size // 2 + 1)

        def fill(begin, end):
            for i in range(begin, end, 2):
                if i == 0:
                    eq.A[i] = self.A[0]
                    eq.B[i] = self.B[0]
                    eq.F[i] = self.F[0]
                elif i == self.size - 1:
                    eq.A[-1] = self.A[-1]
                    eq.C[-1] = self.C[-1]
                    eq.F[-1] = self.F[-1]
                else:
                    k1 = self.C[i - 1] / self.A[i - 1]
                    k2 = self.B[i] / self.A[i + 1]
                    eq.A[i // 2] = self.A[i] - self.B[i - 1] * k1 - self.C[i] * k2
                    eq.C[i // 2 - 1] = - self.C[i - 2] * k1
                    eq.B[i // 2] = - self.B[i + 1] * k2
                    eq.F[i // 2] = self.F[i] - self.F[i - 1] * k1 - self.F[i + 1] * k2

        treads = [
            Thread(target=fill, args=(0, self.size // 4)),
            Thread(target=fill, args=(self.size // 4, self.size // 2)),
            Thread(target=fill, args=(self.size // 2, 3 * self.size // 4)),
            Thread(target=fill, args=(3 * self.size // 4, self.size)),
        ]
        #t1 = datetime.now()
        for t in treads:
            t.start()
        for t in treads:
            t.join()
        #fill(0, self.size)
        #print(f'td {(datetime.now() - t1).total_seconds()}s')
        eq.solve()
        s = eq.solve()
        res = np.zeros(self.size)
        def fill_res(begin, end):
            for i in range(begin, end):
                if i % 2 == 0:
                    res[i] = s[i // 2]
                else:
                    res[i] = (self.F[i] - self.C[i - 1] * s[i // 2] - self.B[i] * s[i // 2 + 1]) / self.A[i]

        treads = [
            Thread(target=fill_res, args=(0, self.size // 4)),
            Thread(target=fill_res, args=(self.size // 4, self.size // 2)),
            Thread(target=fill_res, args=(self.size // 2, 3 * self.size // 4)),
            Thread(target=fill_res, args=(3 * self.size // 4, self.size)),
        ]
        # t1 = datetime.now()
        for t in treads:
            t.start()
        for t in treads:
            t.join()
        #fill_res(0, self.size)
        return res


def calculate_ABC(y0, h, eps=1):
    """
    returns A, B, C : A[0] = B[0] = A[-1] = C[-1] = 0
    """
    size = len(y0)
    A = eps * (-2 * np.ones(size) / (h * h) + np.exp(y0))  # eps = 1 :  eps(y'' - yf'(y0)) = f(y0) - y0'' - y0f'(y0)
    C = eps * np.ones(size - 1) / (h * h)
    B = eps * np.ones(size - 1) / (h * h)
    A[0] = B[0] = A[-1] = C[-1] = 0

    return A, B, C


def d2(y, h):
    res = np.zeros(len(y) - 2)
    for i in range(1, len(y) - 1):
        res[i - 1] = (y[i - 1] - 2 * y[i] + y[i + 1]) / (h * h)
    return res


def d1(y, h):
    res = np.zeros(len(y) - 2)
    for i in range(1, len(y) - 1):
        res[i - 1] = (y[i + 1] - y[i + 1]) / (2 * h)
    return res


def calculate_F(f):
    res = f.copy()
    for i in range(1, len(res) - 1):
        res[i] += (f[i + 1] - 2 * f[i] + f[i - 1]) / 12
    return res


def f_of_y0(y0, h):
    res = np.exp(-y0)
    res[1:-1] -= d2(y0, h)
    return res


def f_of_y(y0):
    res = np.exp(-y0)
    return res


start = datetime.now()
N = 70001
l = 0
r = 1
x = np.linspace(l, r, N)
h = x[1] - x[0]

tr = ThreeDiagonalEq(N)
ans = []
b_arr = np.linspace(1, 0, 11)
for b in b_arr:
    y0 = np.linspace(1, b, N)
    k = 25
    for i in range(k):
        tr.F = calculate_F(f_of_y0(y0, h))
        tr.A, tr.B, tr.C = calculate_ABC(y0, h)
        tr.A[0] = tr.A[-1] = 1
        tr.F[0] = 1 - y0[0]
        tr.F[-1] = b - y0[-1]
        y0 += 0.2 * tr.reduction1step()

    ans.append(y0)
print(f"time = {(datetime.now() - start).total_seconds()}s")
for i in range(len(ans)):
    y0 = ans[i]
    plt.plot(x[6:-6], d2(y0, h)[5:-5] - f_of_y(y0)[6:-6], label=f'b={b_arr[i]:.2f}')
plt.legend()
plt.show()

for i in range(len(ans)):
    y0 = ans[i]
    plt.plot(x, y0, label=f'b={b_arr[i]:.2f}')
plt.legend()
plt.show()
