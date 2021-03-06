import math
import numpy as np
import matplotlib.pyplot as plt


file = "data/256.txt"
txt = np.genfromtxt(file, delimiter = ",")

name = txt[1:2, 0]
name2 = int(name[0])

secs = txt[16:len(txt) - 1, 1]
cross= txt[16:len(txt) - 1, 2]

xbest = [txt[100, 0]]
ybest = [txt[100, 1]]

plt.plot(cross, secs)
plt.plot(xbest, ybest, 'ro')
plt.text(75, 0.055, '(81, 0.055991)')
plt.title("Avg Seconds vs Crossover, Dimension " + str(name2))
plt.xlabel("Crossover")
plt.ylabel("Avg Seconds")
plt.xlim([16, 100])

plt.savefig('plots/' + str(name2) + '.png')