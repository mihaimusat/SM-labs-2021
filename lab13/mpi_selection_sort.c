#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) 
{
	int n, i, j, k, idx, chunk_size, partial_size, rest_size, rank, num_procs;
	int *v;
	int *partial_sort;
	int *rest;

    	MPI_Status status;

    	if(argc < 2) {
        	printf("Usage: mpirun -np <num_procs> %s <input_file>\n", argv[0]);
        	exit(1);
    	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    	if(rank == 0) { 
		FILE *file = fopen(argv[1], "r");
        	fscanf(file, "%d", &n);
        	
		chunk_size = n / num_procs;       		
		if(n % num_procs) {
            		chunk_size++;
        	}

        	v = (int *)malloc((num_procs * chunk_size) * sizeof(int));
        	for(i = 0; i < n; i++) {
            		fscanf(file, "%d", &v[i]);
       		}

        	fclose(file);

        	for(i = n; i < num_procs * chunk_size; i++) {
            		v[i] = 0;
        	}
	}

    	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    	chunk_size = n / num_procs;
    	if(n % num_procs) {
        	chunk_size++;
    	}

    	partial_sort = (int *)malloc(chunk_size * sizeof(int));
    	MPI_Scatter(v, chunk_size, MPI_INT, partial_sort, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	if(n >= chunk_size * (rank + 1)) {
    		partial_size = chunk_size;
	}
	else {
		partial_size = n - chunk_size * rank;
	}

	for(i = 0; i < partial_size - 1; i++) {
		int min_index = i;	
		for(j = i + 1; j < partial_size; j++) {
            		if(partial_sort[j] < partial_sort[min_index]) {
                		min_index = j;
            		}
        	}
		int aux = partial_sort[i];
		partial_sort[i] = partial_sort[min_index];
		partial_sort[min_index] = aux;
    	}
	

	for(idx = 1; idx < num_procs; idx = 2 * idx) {
		if(rank % (2 * idx) != 0) {
            		MPI_Send(partial_sort, partial_size, MPI_INT, rank - idx, 0, MPI_COMM_WORLD);
            		break;
	    	}
        	if(rank + idx < num_procs) {
			if(n >= chunk_size * (rank + 2 * idx)) {
    				rest_size = chunk_size * idx;
			}
			else {
				rest_size = n - chunk_size * (rank + idx);
			}
            		rest = (int *)malloc(rest_size * sizeof(int));
            		MPI_Recv(rest, rest_size, MPI_INT, rank + idx, 0, MPI_COMM_WORLD, &status);

			int* result = (int*) malloc((partial_size + rest_size) * sizeof(int));			
			i = 0, j = 0;
    			for(k = 0; k < partial_size + rest_size; k++) {
        			if(i >= partial_size) {
            				result[k] = rest[j];
            				j++;
        			} 
				else if(j >= rest_size) {
            				result[k] = partial_sort[i];
            				i++;
        			} 
				else if(partial_sort[i] < rest[j]) {
            				result[k] = partial_sort[i];
            				i++;
        			} 
				else { 
            				result[k] = rest[j];
            				j++;
        			}
    			}
            		partial_sort = result;
            		partial_size += rest_size;
        	}
    	}

    	if(rank == 0) {
            	printf("After sorting the input array, the result is: \n");
	    	for(i = 0; i < partial_size; ++i) {
        		printf("%d ", partial_sort[i]);
    	    	}
    	    	printf("\n"); 
    	}

    	MPI_Finalize();
    	return 0;
}
