import pandas as pd
import numpy as np
import seaborn as sns
from matplotlib import pyplot as plt

times = {'n':[], 'time':[], 'type':[]}
with open('../results/res1.txt', 'r') as f:
    for l in f.readlines():
        s = [int(x) for x in l.split()]
        if s:
            times['n'].append(s[0])
            times['n'].append(s[0])
            times['time'].append(np.median(s[1::2])/1e9)
            times['time'].append(-np.median(s[1::2])/1e9)
            times['type'].append('naive')
            times['type'].append('naive_parallel')



