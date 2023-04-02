build:
	nvcc -I./Common  -gencode arch=compute_61,code=sm_61  -c cuda_functions.cu -o cuda_functions.o
	g++  -fopenmp -c input_reading_functions.c -o input_reading_functions.o
	g++ -c findObjectsFunctions.c -o findObjectsFunctions.o
	g++  -fopenmp -c main.cpp -o main.o
	g++  -fopenmp main.o input_reading_functions.o findObjectsFunctions.o cuda_functions.o -L/usr/local/cuda-11.7/targets/x86_64-linux/lib -lcudart -o main_prog

clean:
	rm -f *.o ./main_prog
run:
	./main_prog
