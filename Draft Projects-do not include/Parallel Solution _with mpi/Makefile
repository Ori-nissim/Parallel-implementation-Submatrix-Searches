build:
	mpicc  -fopenmp -c input_reading_functions.c -o input_reading_functions.o
	mpicc  -fopenmp -c findObjectsFunctions.c -o findObjectsFunctions.o
	mpicc -c mpi_communication.c -o mpi_communication.o
	mpicc  -fopenmp -c main.c -o main.o
	mpicc  -fopenmp main.o input_reading_functions.o mpi_communication.o findObjectsFunctions.o -L/usr/local/cuda-11.7/targets/x86_64-linux/lib -lcudart -o main_prog

clean:
	rm -f *.o ./main_prog
run:
	mpirun -np 2 ./main_prog
