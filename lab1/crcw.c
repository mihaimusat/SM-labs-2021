#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define modul(x) ((x) < 0 ? -(x) : (x))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define eps 1e-10

int N, P;
double *A;
double *B;
double *C;
double *D;


// functie care genereaza un numar random in intervalul [min, max]
double get_random_number(int min, int max) { 
     
	return min + (max - min) * ((double)rand() / (double)RAND_MAX);
}

// functie care compara doua numere de tip double
double cmp(double a, double b) {
	
	double c = modul(a);
	double d = modul(b);

	d = max(c, d);

	return d == 0.0 ? 0.0 : modul(a - b) / d;
}

// functie care extrage parametrii din linie de comanda
// N = dimensiunea matricilor, P = numarul de threaduri (trebuie sa fie egal cu N^3)
void getArgs(int argc, char **argv) {
	
	if(argc != 3) {
		printf("Not enough parameters: ./program N P\n");
		exit(1);
	}
	
	N = atoi(argv[1]);
	P = atoi(argv[2]);
}

// functie care aloca dinamic spatiu pentru matricile utilizate
// A, B = matricile date ca input
// C = matricea obtinuta prin paralelizarea inmultirii matricelor A si B
// D = matricea obtinuta in varianta seriala a inmultirii matricelor A si B
void init(int N) {
	
	int i, j, k;
    
	A = malloc(N * N * sizeof(double));
	B = malloc(N * N * sizeof(double));
	C = malloc(N * N * sizeof(double));
	D = malloc(N * N * sizeof(double));
	
	if(A == NULL || B == NULL || C == NULL || D == NULL) {
		printf("Malloc failed!\n");
		exit(1);
	}

	srand(time(NULL));
	for(k = 0; k < N * N; k++) {
		i = k / N;
		j = k % N;
		A[i * N + j] = get_random_number(0.0, 100.0);
		B[i * N + j] = get_random_number(0.0, 100.0);
	}
}

// functie care elibereaza memoria utilizata
void free_memory() {
	
	free(A);
	free(B);
	free(C);
	free(D);
}

int main(int argc, char** argv) {
	
	int i, j, k;
	struct timeval tv1, tv2;
    	struct timezone tz;
   	double elapsed;
    	double sum = 0.0;
    
	getArgs(argc, argv);
	
	init(N);

	// seteaza numarul de thereaduri
	omp_set_num_threads(P);
	
	// start measuring time
	gettimeofday(&tv1, &tz);
	
	// paralelizeaza inmultirea matricelor A si B folosind modelul CRCW-PRAM
    	#pragma omp parallel for private (i, j, k) shared(A, B, C) reduction (+:sum)
    	for(i = 0; i < N; i++) {
        	for(j = 0; j < N; j++) {
        		sum = 0.0;
            		for(k = 0; k < N; k++) {
            			sum += A[i * N + k] * B[k * N + j];
            		}
            		C[i * N + j] = sum;
        	}
    	}
	
	// end measuring time
	gettimeofday(&tv2, &tz);
    	elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;


	// calculeaza inmultirea matricelor A si B in varianta seriala
	for(i = 0; i < N; i++) {
        	for(j = 0; j < N; j++) {
        		D[i * N + j] = 0.0;
            		for(k = 0; k < N; k++) {
                		D[i * N + j] += A[i * N + k] * B[k * N + j];
            		}
        	}
    	}
	
	// verifica daca rezultatul obtinut de algoritmul serial difera de
	// rezultatul obtinut folosind algoritmul paralel, cu cel mult toleranta eps
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			if(cmp(C[i * N + j], D[i * N + j]) > eps) {
				printf("Matrix elements differ!\n");
				free_memory();
				break;
			}
		}
	}
	
	printf("%d, %d, %.5f\n", N, P, elapsed);
	free_memory();
	return 0; 
}

