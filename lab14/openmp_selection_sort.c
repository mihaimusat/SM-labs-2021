#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 4

struct min_info {
	int val;
	int index;
};

#pragma omp declare reduction(minimum : struct min_info : omp_out = omp_in.val < omp_out.val ? omp_in : omp_out) \
			      initializer(omp_priv = omp_orig)

int main(int argc, char *argv[]) 
{
	int n, i, j;
	int *v;

    	if(argc < 2) {
        	printf("Usage: %s <num_threads> <input_file>\n", argv[0]);
        	exit(1);
    	}

	omp_set_num_threads(NUM_THREADS);

	FILE *file = fopen(argv[1], "r");
        fscanf(file, "%d", &n);

        v = (int *)malloc(n * sizeof(int));
        for(i = 0; i < n; i++) {
        	fscanf(file, "%d", &v[i]);
       	}

        fclose(file);

	for(i = 0; i < n - 1; i++) {
		
		struct min_info mini;
		mini.index = i;
		mini.val = v[i];		

		#pragma omp parallel for reduction(minimum : mini)	
		for(j = i + 1; j < n; j++) {
            		if(v[j] < mini.val) {
                		mini.index = j;
				mini.val = v[j];
            		}
        	}
		int aux = v[i];
		v[i] = v[mini.index];
		v[mini.index] = aux;
    	}

        printf("After sorting the input array, the result is: \n");
	for(i = 0; i < n; i++) {
        	printf("%d ", v[i]);
    	}

    	printf("\n"); 

    	return 0;
}
