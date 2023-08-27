import sys
import os

from matplotlib import pyplot as plt

# open all txt files in this directory and draw hists
for file in os.listdir('D:\\produced\\multimedia\\version1\\gist\\'):
    if file.endswith(".txt"):
        x = []
        y = []
        with open(file, "r") as f:
            lines = f.readlines()
            for line in lines:
                x.append(float(line.split()[0]))
                y.append(float(line.split()[1]))
        plt.plot(x, y)
        plt.xlim([-100, 100])
        plt.savefig(file[:-4] + ".png")
        plt.close()

