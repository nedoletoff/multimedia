import os

import matplotlib.pyplot as plt
import seaborn as sns
import numpy
import numpy as np


def to_int_tuple(s: str):
    return [int(x) for x in s.split(' ')]


def draw3(data_file: str, color: str):
    with open(data_file, 'r') as file:
        data = [line.strip() for line in file]
        print(data)
        data = [to_int_tuple(x) for x in data]
        print(data)
        _range = (-255, 255)
        data = [x[1] for x in data]
        sns_ = sns.distplot(data)
        fig = sns_.get_figure()
        fig.savefig(color + ".png")


def draw2(data_file: str, color: str):
    with open(data_file, 'r') as file:
        data = [line.strip() for line in file]
        print(data)
        data = [to_int_tuple(x) for x in data]
        print(data)
        _range = (-255, 255)
        data = [x[1] for x in data]
        print(data)
        plt.hist(data, range=_range, facecolor=color, alpha=0.75,
                 edgecolor=color, bins=511)

        plt.title(data_file)
        plt.show()


def draw(data_file: str, color: str):
    with open(data_file, 'r') as file:
        data = [line.strip() for line in file]
        data = [to_int_tuple(x) for x in data]
        data1 = [x[1] for x in data]
        data0 = [x[0] for x in data]
        _range = (-255, 255)
        plt.plot(data0, data1, color=color)
        plt.title(data_file)
        path = "hist/"
        if not os.path.exists(path):
            os.mkdir(path)
        plt.savefig(path + data_file + ".png")
        plt.close()


def main():
    files = ["count_B", "count_G", "count_R",
             "count_Y", "count_Cb", "count_Cr"]

    colors = ["blue", "green", "red", "yellow", "cyan", "magenta"]
    for i in range(len(files)):
        for j in range(4):
            if j == 0:
                draw(files[i] + ".txt", colors[i])
            else:
                draw(files[i] + str(j) + ".txt", colors[i])


if __name__ == "__main__":
    main()
