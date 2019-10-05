import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from collections import deque

PC, Hz, ET_c, P5_c, M3_c, m3_c, P5_b, M3_b, m3_b = ([] for _ in range(9))

circleof5ths = [(7*i) % 12 for i in range(12)]
data = pd.read_csv("kirnberger3.dat", sep="\t")
data = data.reindex(circleof5ths)

PC_names = ["C", "G", "D", "A", "E", "B", "F\u266f", "C\u266f", "G\u266f", "E\u266d", "B\u266d", "F"]
PC   = data.loc[:, "PC"]
P5_c = data.loc[:, "P5/c"]
M3_c = data.loc[:, "M3/c"]

m3_c = deque(abs(data.loc[:, "m3/c"]))
m3_c.rotate(-4)

width = 0.3
ind = np.arange(12)
plt.bar(ind, M3_c, width, align="center", label="Major 3rd")
plt.bar(ind + width, m3_c, width, label="Minor 3rd")
plt.bar(ind + 1/2*width, P5_c, width, label="Perfect 5th")
plt.xticks(ind, PC_names)
plt.xlabel("Major triad")
plt.ylabel("Cents")
plt.title("Kirnberger III")
plt.legend(loc="best")
plt.savefig("kirnberger3.pdf")

