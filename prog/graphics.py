import pandas as pd
import numpy as np
import pylab as pl

fig = pl.figure()
def plotty(dir, name, add):
	read_df = pd.read_csv( dir + name,sep = "\t")
	t = read_df[[0]].values
	x = read_df[[1]].values
	MyPlot(a = t,b = x,name_1 ="x",name_2 ="u", file = "result/"+ add + name)

def MyPlot(a,b, x_0 = -1, x_1 = 1, y_0 = -0.5, y_1 = 1.5, name_1 = "t", name_2 = "x", file ="file"):
	pl.plot(a, b)
	pl.axis([x_0, x_1 , y_0, y_1])
	pl.xlabel(name_1)
	pl.ylabel(name_2)
	#pl.title('Simple box whisker chart')
	pl.grid(True)

#plotty("calculations1/", "1.txt", "1")
#plotty("calculations1/", "1.txt", "1")
#plotty("anal/", "8000.txt", "2")
plotty("anal/", "1.txt", "2")
#plotty("anal/", "2ott.txt", "2")
plotty("", "l1.csv", "2")
#plotty("", "l2.csv", "2")

#pl.savefig('ends.png')
pl.savefig('enn.png')

#k = np.arange(0,20,1)
#for i in k:
#	plotty(str(i)+".txt")