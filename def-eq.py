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

        N = len(self.A)
        mat = np.zeros((N, N))
        for i in range(N - 1):
            mat[i][i] = self.A[i]
            mat[i][i+1] = self.B[i]
            mat[i +1][i] = self.C[i]
        mat[-1][-1] = self.A[-1]
        print(mat)

        xx = np.linalg.solve(mat, self.F)

        for i in range(1, self.size):
            A1[i] -= self.C[i - 1] * self.B[i - 1] / A1[i - 1]
            F1[i] -= self.C[i - 1] * F1[i-1] / A1[i - 1]
        F1[-1] /= A1[-1]

        mat2 = np.zeros((N, N))
        for i in range(N - 1):
            mat2[i][i] = A1[i]
            mat2[i][i + 1] = self.B[i]
        mat2[-1][-1] = A1[-1]
        print(mat2)
        for i in range(2, self.size + 1):
            F1[-i] = (F1[-i] - self.B[-i + 1] * F1[-i + 1]) / A1[-i]
        return F1


def calculate_F(x: np.ndarray):
    return np.sin(x)


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
    return x * math.sin(1) - np.sin(x)  # (x - 10)*e10/20 + np.exp(-x) + (x + 10)/(20*e10) + 2


def d2(y, h):
    y0 = np.zeros(len(y) - 2)
    for i in range(1, len(y) - 1):
        y0[i - 1] = (y[i - 1] - 2 * y[i] + y[i + 1]) / (h * h)
    return y0

N = 25
x = np.linspace(-1, 1, N)
x1 = np.linspace(-1, 1, 1000)

tr = ThreeDiagonalEq(N)
tr.F = calculate_F(x)
tr.A, tr.B, tr.C = calculate_ABC(x[1] - x[0], N)
tr.A[0] = tr.A[-1] = 1
tr.F[0] = tr.F[-1] = 0

y0 = tr.solve()

print(tr.B)
print(tr.A)
print(tr.C)

plt.plot(x1, solution(x1))
plt.plot()
plt.plot(x, y0)
plt.show()

plt.plot(x[1:-1], tr.F[1:-1])
plt.plot(x[1:-1], d2(y0, x[1] - x[0]))
plt.show()
