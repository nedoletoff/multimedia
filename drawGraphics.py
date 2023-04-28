import matplotlib.pyplot as plt

p = [x / 10 for x in range(0, 11)]
with open("out.txt", "r") as f:
    lines = f.readlines()
    bf = lines[0].split(",")[:-1]
    bf = [float(x) for x in bf]
    sm = lines[1].split(",")[:-1]
    sm = [float(x) for x in sm]
    fsm = lines[2].split(",")[:-1]
    fsm = [float(x) for x in fsm]
plt.plot(p, bf, label="BF")
plt.plot(p, sm, label="SM")
plt.plot(p, fsm, label="FSM")
plt.legend()
plt.show()