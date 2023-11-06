import matplotlib.pyplot as plt
import seaborn as sns

data = [[0 for i in range(20)] for j in range(10)]
with open("results", 'r') as f:
    s = f.read()
    i = 0
    j = 0
    k = 0
    for line in s.split('\n'):
      if line:
        if line[0] == 'A':
          _, _, time = line.split(' ')
          data[9-i%10][j%20] = float(time)
          k += 1
          j += 1
          if k == 20:
            k = 0
            i += 1


plt.figure(figsize=(12, 8))
cmap = sns.cm.rocket_r
sns.heatmap(data, cmap=cmap, xticklabels=range(100, 2001, 100), yticklabels=range(20, 0, -2))
plt.ylabel('Number of procs')
plt.xlabel('Number of tasks')
plt.title(f'Execution time, ms')
plt.savefig("heatmap.png", bbox_inches='tight')