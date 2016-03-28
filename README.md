# Prims-Matrix-Solver
A QT graphical application for finding a minimum spanning tree

###INSTALL
You can open this in QT Creator, it will ask you what kit you want to use. It has worked with all kits I have tested thus far

###How To Use
Open the app
Input the number of nodes in your network
Press the set Nodes button
A table shall magically appear!
Input your network as a matrix, it will auto compleate the other half of the matrix for you.
If two nodes are not connected, input a dash.

Press find MST

it will give you the mst in the form a->b where a and b are connected nodes

###What doesn't work yet
It only works with whole numbers under 1000 so far. This is what I will change next.
It only works for weighted graphs, not weighted digraphs. This feature is to come.
It doesn't yet give you the total weight of the MST.
