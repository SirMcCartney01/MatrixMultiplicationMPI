#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>

#define n 2048		//Tamaño de la matriz

//Creando matrices
int 
  A[n][n], 
  B[n][n], 
  C[n][n];

void fill_matrix(int m[n][n])
{
  int i, j;
  for (i=0; i<n; i++){
    for (j=0; j<n; j++){
      m[i][j] = rand()%(2000 + 1 - 1000)+1000; //Llenamos con valores pseudoaleatorios entre 1000 y 2000
    }
  }
}

int main(int argc, char *argv[])
{
  int l,P;
  double average = 0.0;
  MPI_Status status;  
  MPI_Init (&argc, &argv);
  for(l = 0; l < 100; l++){ //Iteramos 100 veces
  
    clock_t start = clock();
    int myrank, from, to, i, j, k;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);


    if (n%P!=0) {
      if (myrank==0) 
        printf("El tamaño de la matriz no es divisible entre el numero de procesadores\n");
      MPI_Finalize();
      exit(-1);
    }

    from = myrank * n/P;
    to = (myrank+1) * n/P;


    if (myrank==0) {
      fill_matrix(A);
      fill_matrix(B);
    }

    MPI_Bcast (B, n*n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter (A, n*n/P, MPI_INT, A[from], n*n/P, MPI_INT, 0, MPI_COMM_WORLD);

    //printf("Resolviendo particion %d (desde el elemento %d hasta el %d)\n", myrank, from, to-1);
    for (i=from; i<to; i++){
      for (j=0; j<n; j++) {
        for (k=0; k<n; k++){
          C[i][j] += A[i][k]*B[k][j];
        }
      }
    } 
    MPI_Gather (C[from], n*n/P, MPI_INT, C, n*n/P, MPI_INT, 0, MPI_COMM_WORLD);

    //printf("Tiempo transcurrido: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
    average += ((double)clock() - start) / CLOCKS_PER_SEC; 
    
  }
 //Memory usage
  struct rusage r_usage;
  getrusage(RUSAGE_SELF,&r_usage);
  printf("Uso de memoria: %ld bytes\n",r_usage.ru_maxrss);


  average = ((average/100)/.60);
  MPI_Finalize();
  printf("Promedio: %f\n",(average));
  return 0;
}




