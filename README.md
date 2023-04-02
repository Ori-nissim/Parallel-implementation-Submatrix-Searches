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

