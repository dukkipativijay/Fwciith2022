#Python libraries for math and graphics
import random as rd
import numpy as np
import matplotlib.pyplot as plt
from numpy import linalg as LA
import math


import sys                                          #for path to external scripts
sys.path.insert(0,'/sdcard/Download/CoordGeo')         #path to my scripts

#local imports
from line.funcs import *
from triangle.funcs import *
from conics.funcs import circ_gen

#if using termux
import subprocess
import shlex
#end if

#Input parameters
r = 6       #radius
d = 10      #distance of points from center
O = np.array([0,0])   #center
e1 = np.array(([1,0]))

A = d*e1

theta =  np.arcsin(r/d)

e1 = np.array([[1],[0]])
u = np.array([[0],[0]])
v = np.array([[1,0],[0,1]])
h = np.array([[10],[0]])
R = np.array([[np.cos(theta),-np.sin(theta)],[np.sin(theta),np.cos(theta)]])

v = np.linalg.inv(v)

f = -36

m = R @ h
q = e1 / (e1.T @ h)

ui1 = ( 1 / (m.T @ v @ m ) ) * ( -m.T @ ( ( v @ q ) + u ) + math.sqrt( np.square([m.T @ ( ( v @ q ) + u ) ]) - [ ( q.T @ v @ q + 2 * u.T @ q + f )@(m.T @ v @ m )]))
ui2 = ( 1 / (m.T @ v @ m ) ) * ( -m.T @ ( ( v @ q )+ u ) - math.sqrt( np.square([m.T @ ( ( v @ q ) + u ) ]) - [ ( q.T @ v @ q + 2 * u.T @ q + f )@(m.T @ v @ m )]))


n1 = q + ui1 * R @ h

f0 = u.T @ v @ u - f

ki1 = math.sqrt ( f0 / ( n1.T @ v @ n1 ))
ki2 = -math.sqrt ( f0 / ( n1.T @ v @ n1 ))

qi1 = v @ ( ki1 * n1 - u )
qi2 = v @ ( ki2 * n1 - u )


qi1 = qi1[::-1].T @ np.array([[1,0],[0,1]])
qi2 = qi2[::-1].T @ np.array([[-1,0],[0,1]])


P1 = np.concatenate([i for i in np.array(qi1)])
P2 = np.concatenate([i for i in np.array(qi2)])

print('point of contact p1 = ',P1)
print('point of contact p2 = ',P2)

L1 = P1 - h.T
print('Length of Tangent 1 =', np.linalg.norm(L1))

L2 = P2 - h.T
print('Length of Tangent 2 =', np.linalg.norm(L2))

##Generating all lines
xAB = line_gen(O,A)

xPP1 = line_gen(A,P1)
xPP2 = line_gen(A,P2)


##Generating the circle
x1_circ= circ_gen(O,r)

#Plotting all lines
plt.plot(xAB[0,:],xAB[1,:])

plt.plot(xPP1[0,:],xPP1[1,:],label='$Tangent1$')
plt.plot(xPP2[0,:],xPP2[1,:],label='$Tangent2$')


#Plotting the circle
plt.plot(x1_circ[0,:],x1_circ[1,:],label='$Circle$')

#Labeling the coordinates
tri_coords = np.vstack((O,P1,P2,A)).T
plt.scatter(tri_coords[0,:], tri_coords[1,:])
vert_labels = ['O','P1','P2','A']
for i, txt in enumerate(vert_labels):
    plt.annotate(txt, # this is the text
                 (tri_coords[0,i], tri_coords[1,i]), # this is the point to label
                 textcoords="offset points", # how to position the text
                 xytext=(-5,10), # distance from text to points (x,y)
                 ha='center') # horizontal alignment can be left, right or center



plt.xlabel('$x-axis$')
plt.ylabel('$y-axis$')
#plt.legend(loc='best')
plt.grid() # minor
plt.axis('equal')


plt.savefig('/sdcard/Download/circlefig.pdf')
subprocess.run(shlex.split("termux-open '/sdcard/Download/circlefig.pdf'"))

plt.show()
