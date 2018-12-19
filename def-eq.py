"""
solves def eg y'' - e^-y = 0 ; y(0) =0;  y(1) = b
"""
import numpy as np
import math
import matplotlib.pyplot as plt
from datetime import datetime, timedelta


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


def calculate_ABC(y0, h, eps = 1):
    """
    returns A, B, C : A[0] = B[0] = A[-1] = C[-1] = 0
    """
    size = len(y0)
    A = eps*(-2 * np.ones(size) / (h * h) + np.exp(y0))  # eps = 1 :  eps(y'' - yf'(y0)) = f(y0) - y0'' - y0f'(y0)
    C = eps*np.ones(size - 1) / (h * h)
    B = eps*np.ones(size - 1) / (h * h)
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
N = 1000
l = 0
r = 1
x = np.linspace(l, r, N)
h = x[1] - x[0]

tr = ThreeDiagonalEq(N)
ans = []
b_arr = np.linspace(1, 0, 11)
for b in b_arr:
    y0 = np.linspace(1, b, N)
    k = 150
    for i in range(k):
        tr.F = calculate_F(f_of_y0(y0, h))
        tr.A, tr.B, tr.C = calculate_ABC(y0, h)
        tr.A[0] = tr.A[-1] = 1
        tr.F[0] = 1 - y0[0]
        tr.F[-1] = b - y0[-1]
        y0 += 0.2*tr.solve()

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