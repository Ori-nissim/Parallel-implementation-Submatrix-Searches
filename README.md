#  Parallel implementation of Submatrix Search - Parallel and Distributed Computation
### TL;DR
We have an input file that contains pictures and objects that are matrices in different sizes.
The job is to search in every position(i,j) for a match with any of the objects. It consists of heavy work for a single processor yet parallelizing the work 
cuts almost half of the runtime!
For better understanding:
![Screenshot 2023-04-02 162124](https://user-images.githubusercontent.com/93268216/229355523-9931a24a-6d21-4e31-82b3-08f4d3772884.png)

### What are the important parts in the folder?
* Parallel Solution - Can be ran on two computers with MPI communication.
* Sequntial Solution - Demonstartion of how heavy this work is and better understanding of the problem.
* Documentation of my work and Complexity analysis

## Further details about the project 
### Problem Definition

Picture(N) and Object(N) – are square matrices of integers with N rows and N columns. The project deals with a sets of Pictures and Objects of different sizes. Each member of the matrix represents a “color”. The range of possible colors is [1, 100].
Position(I, J) defines a coordinates of the upper left corner of the Object into Picture. 
For each pair of overlapping members p and o of the Picture and Object we will calculate a relative difference
				diff = abs((p – o)/p)
The total difference is defined as an average of all relative differences for all overlapping members for given Position(I, J) of the Object into Picture. We will call it Matching(I, J).
For example, for the Picture and Object from the Fig.1 the matching at Position(0,0) is equal
Matching(0,0) = (abs((10-5)/10) + abs((5-14)/5) + abs((67-9)/67) + abs((23-20)/23) + abs((6-56)/6) +
abs((5-2)/5) + abs((12-6)/12) + abs((10-10)/10) + abs((20-3)/20))/9

