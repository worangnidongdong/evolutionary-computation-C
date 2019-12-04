import random as rd
import matplotlib.pyplot as plt

def fx(x):
	return 3 * x[0] + 4 * x[1] + 5

def generate():
	for i in range(20):
		x1 = rd.random() * 20 - 10
		x2 = rd.random() * 20 - 10

		d = fx([x1,x2])
		if d >= 0:
			xl = 1
		else:
			xl = 0

		print('%.2f %.2f %d' % (x1, x2, xl))

def dplot():
	with open('data.txt') as fp:
		d = []
		l = fp.readline().strip()
		l = l.split(' ')
		ipos = len(l) - 1
		dpos = 0
		while len(l) > 2:
			d.append([float(dd) for dd in l])
			d[dpos][ipos] = int(d[dpos][ipos])
			l = fp.readline().strip()
			l = l.split(' ')
			dpos = dpos + 1

	x1r = []
	x2r = []
	x1l = []
	x2l = []

	for dd in d:
		if dd[2] == 1:
			x1r.append(dd[0])
			x2r.append(dd[1])
		else:
			x1l.append(dd[0])
			x2l.append(dd[1])

	plt.figure()
	plt.scatter(x1r, x2r, c = '#ff0000', s = 20, marker = 'o')
	plt.scatter(x1l, x2l, c = '#00ff00', s = 20, marker = '*')
	plt.show()



if __name__ == '__main__':
	dplot()