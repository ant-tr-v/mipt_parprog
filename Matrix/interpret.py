import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.linear_model import Lasso
import re

times = {'n': [], 'time': [], 'type': []}
with open('./results/res1.txt', 'r') as f:
    for l in f.readlines():
        m = re.match(r'(\S+)\s+(\d+)\s+((\d+\s*)+)', l)
        if m:
            times['n'].append(int(m.group(2)))
            times['type'].append(m.group(1))
            times['time'].append(np.median([int(x) for x in m.group(3).split()])
                                 * 1e-9)

data = pd.DataFrame(data=times)
data['n3'] = 1.*data.n ** 3

types = set(times['type'])
arr = []
for t in types:
    lr = Lasso()
    dat = data[data.type == t]
    lr.fit(dat.drop(columns=['n', 'type', 'time']).values, dat.time.values)
    arr.append((t, lr.coef_[0]))

print('\n'.join(['{}: {}'.format(s[0], s[1]) for s in sorted(arr, key=lambda x: x[1])]))

sns.scatterplot('n3', 'time', hue='type', data=data)
plt.show()




