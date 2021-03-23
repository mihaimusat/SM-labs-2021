import matplotlib
import matplotlib.pyplot as plt
import numpy as np

labels_n = ["E(n)", "U(n)", "S(n)", "R(n)", "Q(n)"]
labels_p = ["E(p)", "U(p)", "S(p)", "R(p)", "Q(p)"]

def plot_func_n(ax, start, end, f):
	samples = 100.0
	spacing = (end - start) / samples

	vf = np.vectorize(f)

	x = np.arange(start, end, spacing)
	y = vf(x)

	ax.plot(x, y)
	ax.legend(labels_n, loc='upper right')

def plot_func_p(ax, start, end, f):
	samples = 100.0
	spacing = (end - start) / samples

	vf = np.vectorize(f)

	x = np.arange(start, end, spacing)
	y = vf(x)

	ax.plot(x, y)
	ax.legend(labels_p, loc='upper left')


T = lambda p, n: 2 * (n / p) * np.log2(n / p) + 3 * (n / p) * np.log2(p) + np.log2(p) ** 2
O = lambda p, n: n * np.log2(n) + n * np.log2(p) + np.log2(p) ** 2

S = lambda p, n: T(1, n) / T(p, n)
E = lambda p, n: S(p, n) / p
R = lambda p, n: O(p, n) / O(1, n)
U = lambda p, n: R(p, n) * E(p, n)
Q = lambda p, n: S(p, n) * E(p, n) / R(p, n)

fig, ax = plt.subplots(2, 3)
plt.subplots_adjust(wspace = 0.4, hspace = 0.4)

n = 16
start = 1
end = 32

ax1 = ax[0][0]

ax1.set(xlabel='Number of processors(p)', ylabel='E, U, S, R, Q',
       title='Performances when keeping n = ' + str(n))

plot_func_p(ax1, start, end, lambda p: E(p, n))
plot_func_p(ax1, start, end, lambda p: U(p, n))
plot_func_p(ax1, start, end, lambda p: S(p, n))
plot_func_p(ax1, start, end, lambda p: R(p, n))
plot_func_p(ax1, start, end, lambda p: Q(p, n))

n = 32
start = 1
end = 32

ax1 = ax[0][1]

ax1.set(xlabel='Number of processors(p)', ylabel='E, U, S, R, Q',
       title='Performances when keeping n = ' + str(n))

plot_func_p(ax1, start, end, lambda p: E(p, n))
plot_func_p(ax1, start, end, lambda p: U(p, n))
plot_func_p(ax1, start, end, lambda p: S(p, n))
plot_func_p(ax1, start, end, lambda p: R(p, n))
plot_func_p(ax1, start, end, lambda p: Q(p, n))

n = 64
start = 1
end = 32

ax1 = ax[0][2]

ax1.set(xlabel='Number of processors(p)', ylabel='E, U, S, R, Q',
       title='Performances when keeping n = ' + str(n))

plot_func_p(ax1, start, end, lambda p: E(p, n))
plot_func_p(ax1, start, end, lambda p: U(p, n))
plot_func_p(ax1, start, end, lambda p: S(p, n))
plot_func_p(ax1, start, end, lambda p: R(p, n))
plot_func_p(ax1, start, end, lambda p: Q(p, n))

p = 16
start = 1
end = 32

ax1 = ax[1][0]

ax1.set(xlabel='Size of matrix(n)', ylabel='E, U, S, R, Q',
       title='Performances when keeping p = ' + str(p))

plot_func_n(ax1, start, end, lambda n: E(p, n))
plot_func_n(ax1, start, end, lambda n: U(p, n))
plot_func_n(ax1, start, end, lambda n: S(p, n))
plot_func_n(ax1, start, end, lambda n: R(p, n))
plot_func_n(ax1, start, end, lambda n: Q(p, n))

p = 32
start = 1
end = 32

ax1 = ax[1][1]

ax1.set(xlabel='Size of matrix(n)', ylabel='E, U, S, R, Q',
       title='Performances when keeping p = ' + str(p))

plot_func_n(ax1, start, end, lambda n: E(p, n))
plot_func_n(ax1, start, end, lambda n: U(p, n))
plot_func_n(ax1, start, end, lambda n: S(p, n))
plot_func_n(ax1, start, end, lambda n: R(p, n))
plot_func_n(ax1, start, end, lambda n: Q(p, n))

p = 64
start = 1
end = 32

ax1 = ax[1][2]

ax1.set(xlabel='Size of matrix(n)', ylabel='E, U, S, R, Q',
       title='Performances when keeping p = ' + str(p))

plot_func_n(ax1, start, end, lambda n: E(p, n))
plot_func_n(ax1, start, end, lambda n: U(p, n))
plot_func_n(ax1, start, end, lambda n: S(p, n))
plot_func_n(ax1, start, end, lambda n: R(p, n))
plot_func_n(ax1, start, end, lambda n: Q(p, n))

plt.show()
