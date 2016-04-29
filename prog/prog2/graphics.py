import pandas as pd
import numpy as np
import pylab as pl

read_df = pd.read_csv("out.csv",sep = "\t")


t = read_df[[0]].values
x = read_df[[1]].values
y = read_df[[2]].values

def MyPlot(a,b, x_0 = 0, x_1 = 30, y_0 = -2, y_1 = 2, name_1 = "t", name_2 = "x", file ="file"):
	fig = pl.figure()
	pl.plot(a, b)
	pl.axis([x_0, x_1 , y_0, y_1])
	pl.xlabel(name_1)
	pl.ylabel(name_2)
	pl.title('Simple box whisker chart')
	pl.grid(True)
	pl.savefig(file + '.pdf')


MyPlot(a = t,b = x,name_1 ="t",name_2 ="x", file = "x-t")
MyPlot(a = t,b = y,name_1 ="t",name_2 ="y",file = "y-t")
MyPlot(a = x,b = y,x_0 = -2, x_1 = 2,name_1 ="x",name_2 ="y",file = "x-y")
