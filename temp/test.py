import math
import matplotlib.pyplot as plt


def bernuli(n: int, k: int, p: float):
    return math.comb(n, k) * p ** k * (1 - p) ** (n - k)


def draw_bernuli(n: int, k: int, p: float):
    plt.bar(range(0, n), bernuli(n, k, p))
    plt.show()


def main():
    n = 57
    k = 10
    p = 0.5
    draw_bernuli(n, k, p)
