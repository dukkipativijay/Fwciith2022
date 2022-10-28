from sympy import*

import cvxpy as cp
import sympy as sy
import numpy as np
import matplotlib.pyplot as plt
from numpy import linalg as LA
import math
import mpmath as mp

#if using termux
import subprocess
import shlex
#end if

print("Verification Using Differentiation")

r = symbols ('r',real=True)
x = symbols ('x',real=True)
pi = symbols ('pi',real=True)
k = symbols ('k',real=True)

v = (2/3)* x**3 + (4/3)*pi*r**3


print("V =", v)

v1 = (2/3)* x**3  + (4/3) * pi * sy.sqrt(((k - 6* x**2)/(4 * pi))**3)

k1 = 6* x**2 + 4*pi* r**2

df = diff(v1, x)

print("dV/dx =")
print(df)

sol = sy.solve(df.subs(k,k1),x)[1]
print("when dv/dx = 0, x =")
print(sol)

volume = sy.simplify(v.subs(x,r*3))
print("we get the Minimum volume which is =")
print(sy.simplify(volume))

print("Verification Using DGP cvxpy")

x = cp.Variable(pos=True, name="x")
r = cp.Variable(pos=True, name="r")

#k = int(input("Enter k: "))
k=40

#Computing surface area
V = (2/3)* np.pi*(x**3) + (4/3)*np.pi*(r**3)


constraints = [
        x == 3*r,
        6* x + 4* np.pi * r == k 
]

#Problem Formulation
problem = cp.Problem(cp.Minimize(V), constraints)

#Checking cuvature of the objective function
print(V.log_log_curvature)

#Checking if the problem is DGP
print("Is this problem DGP?", problem.is_dgp())


#solution
problem.solve(gp=False)
print("Min Volume is:",problem.value,"x =", x.value,"r =", r.value)


#plt.savefig('/sdcard/Download/optadv.pdf')
#subprocess.run(shlex.split("termux-open '/sdcard/Download/optadv.pdf'"))
