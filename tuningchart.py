import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from collections import deque

circleof5ths = [(7*i) % 12 for i in range(12)]
data = pd.read_csv("meantone4.dat", sep="\t")
data = data.reindex(circleof5ths)

PC_names = ["C", "G", "D", "A", "E", "B", "F\u266f", "D\u266d", "A\u266d", "E\u266d", "B\u266d", "F"]
PC   = data.loc[:, "PC"]
P5_c = data.loc[:, "P5/c"]
M3_c = data.loc[:, "M3/c"]

m3_c = deque(abs(data.loc[:, "m3/c"]))
m3_c.rotate(-4)

width = 0.3
ind = np.arange(12)
plt.bar(ind - 1/2*width, M3_c, width, align="center", label="M3")
plt.bar(ind + 1/2*width, m3_c, width, label="m3")
plt.bar(ind +   0*width, P5_c, width, label="P5")
plt.xticks(ind, PC_names)
plt.xlabel("Major triad")
plt.ylabel("Cents")
plt.title("Quarter-comma meantone")
plt.legend(loc="best")
plt.savefig("meantone4.pdf")

