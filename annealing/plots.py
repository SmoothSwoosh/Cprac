import matplotlib.pyplot as plt

proc_nums = []
scores = []
times = []

with open('parallel_results', 'r') as f:
	s = f.read()
	prev_proc = False
	for line in s.split('\n'):
		if not line:
			continue
		if line.startswith('Nproc'):
			prev_proc = True
			_, proc_num = line.split(' ')
			proc_nums.append(int(proc_num))
		elif prev_proc:
			prev_proc = False
			scores.append(int(line))
		else:
			times.append(float(line) / 1000)

plt.figure(figsize=(12, 8))
plt.plot(proc_nums, times)
plt.xlabel('Number of threads')
plt.ylabel('Execution time, s')
plt.title('Execution time (number of threads)')
plt.savefig("exec_time.png", bbox_inches='tight')

plt.figure(figsize=(12, 8))
plt.plot(proc_nums, scores)
plt.xlabel('Number of threads')
plt.ylabel('Criterion value')
plt.title('Criterion value (number of threads)')
plt.savefig("scores.png", bbox_inches='tight')
