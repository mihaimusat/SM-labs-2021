#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 4

struct my_thread {
	int *v; 
	int len;
};

void *thread_function(void *arg) {

	struct my_thread* t = arg;
	
	int i, j;

	for(i = 0; i < t->len - 1; i++) {
		int min_index = i;	
		for(j = i + 1; j < t->len; j++) {
            		if(t->v[j] < t->v[min_index]) {
                		min_index = j;
            		}
        	}
		int aux = t->v[i];
		t->v[i] = t->v[min_index];
		t->v[min_index] = aux;
    	}
}

int main(int argc, char *argv[]) 
{
	int n, i, j, idx, chunk_size;
	int *v;

    	if(argc < 2) {
        	printf("Usage: %s <input_file>\n", argv[0]);
        	exit(1);
    	}

	pthread_t tid[NUM_THREADS];
    	struct my_thread thread[NUM_THREADS];

	FILE *file = fopen(argv[1], "r");
        fscanf(file, "%d", &n);
        	
	chunk_size = n / NUM_THREADS;       		
	if(n % NUM_THREADS) {
            	chunk_size++;
        }

        v = (int *)malloc(n * sizeof(int));
        for(i = 0; i < n; i++) {
            	fscanf(file, "%d", &v[i]);
       	}

        fclose(file);

	for(i = 0; i < NUM_THREADS; i++) {
        	if(i != NUM_THREADS - 1) {
            		thread[i].len = chunk_size;
            		thread[i].v = (int *)malloc(thread[i].len * sizeof(int));
        	} 
		else {
            		thread[i].len = n - (NUM_THREADS - 1) * chunk_size;
            		thread[i].v = (int *)malloc(thread[i].len * sizeof(int));
        	}

       		for(j = chunk_size * i; j < chunk_size * (i + 1); j++) {
            		if(j >= n) {
                		break;
            		}
            		thread[i].v[j % chunk_size] = v[j];
        	}
    	}

	for(i = 0; i < NUM_THREADS; i++) {
		int rc = pthread_create(&tid[i], NULL, thread_function, (void *)&thread[i]);
		if(rc) {
		    fprintf(stderr, "Error when creating threads");
		    exit(EXIT_FAILURE);
		}
	}

	for(i = 0; i < NUM_THREADS; i++) {
		pthread_join(tid[i], NULL);
	}

	int num_iters = (int)sqrt(NUM_THREADS);
	num_iters = (num_iters * num_iters == NUM_THREADS) ? (num_iters + 1) : (num_iters + 2);

	int count = 2;
	int k = 1;	

	while(k < num_iters) {

		for(i = 0; i < NUM_THREADS; i += count) {

		    int step = (int)sqrt(count);

		    if(i < NUM_THREADS - k) {
		        
			int dim = thread[i].len + thread[i + step].len;
			int* result = (int*)malloc(dim * sizeof(int));			
			
			int x = 0, y = 0;
    			for(idx = 0; idx < dim; idx++) {
        			if(x >= thread[i].len) {
            				result[idx] = thread[i + step].v[y];
            				y++;
        			} 
				else if(y >= thread[i + step].len) {
            				result[idx] = thread[i].v[x]; 
            				x++;
        			} 
				else if(thread[i].v[x] < thread[i + step].v[y]) {
            				result[idx] = thread[i].v[x];
            				x++;
        			} 
				else { 
            				result[idx] = thread[i + step].v[y];
            				y++;
        			}
    			}
		        thread[i].v = (int *)malloc(dim * sizeof(int));
		        thread[i].len = dim;
		        thread[i].v = result;
		    }
		}
		count *= 2;
		k++;
	}
	
	printf("After sorting the input array, the result is: \n");
	for(i = 0; i < thread[0].len; ++i) {
        	printf("%d ", thread[0].v[i]);
    	}
    	printf("\n"); 

    	return 0;
}
