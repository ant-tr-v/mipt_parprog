"""
solves def eg y'' = a(y-y^3) ; y(-10) = y(10) = sqt(2)
"""
import numpy as np
import math
import matplotlib.pyplot as plt


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


def calculate_F(f):
    res = f.copy()
    for i in range(1, len(res) - 1):
        res[i] += (f[i + 1] - 2 * f[i] + f[i - 1]) / 12
    return res


def calculate_ABC(h, size):
    """
    returns A, B, C : A[0] = B[0] = A[-1] = C[-1] = 0
    """
    A = -2 * np.ones(size) / (h * h)
    C = np.ones(size - 1) / (h * h)
    B = np.ones(size - 1) / (h * h)
    A[0] = B[0] = A[-1] = C[-1] = 0

    return A, B, C


def solution(x):
    return -x ** 4 / 1200 + 1./10 * x *(5 + math.sin(10)) -  np.sin(x) + 40./3


def d2(y, h):
    y0 = np.zeros(len(y) - 2)
    for i in range(1, len(y) - 1):
        y0[i - 1] = (y[i - 1] - 2 * y[i] + y[i + 1]) / (h * h)
    return y0


N = 25
a = -10
b = 10
x = np.linspace(a, b, N)
h = x[1] - x[0]
x1 = np.linspace(a, b, 1000)

tr = ThreeDiagonalEq(N)
tr.F = calculate_F(np.sin(x) - x * x / 100)
tr.A, tr.B, tr.C = calculate_ABC(h, N)
tr.A[0] = tr.A[-1] = 1
tr.F[0] = 0
tr.F[-1] = 10

y0 = tr.solve()

plt.plot(x1, solution(x1))
plt.plot()
plt.plot(x, y0)
plt.show()
