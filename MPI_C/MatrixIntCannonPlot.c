#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define n 512		//Tamaño de la matriz

//Plot crap
enum { MAXL = 8, MAXC = 256};

FILE *xfopen (const char *fn, const char *mode);
int badmode (const char *s);
int xfclose (FILE *fp);
int xfexists (char *fn);
char *fnwoext (char *nm, char *fn);

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

      //CANNON SHIT

    double kk;
    int vecino_derecho, vecino_izquierdo, vecino_arriba, vecino_abajo;

    int PP = P;
    int x;
    double np = P;
    kk = sqrt(np);
    k = (int)kk;
    if (myrank < k) // below neighbour set
    {
        vecino_izquierdo = (myrank + k - 1) % k;
        vecino_derecho = (myrank + k + 1) % k;
        vecino_arriba = ((k - 1)*k) + myrank;
    }
    if (myrank == k)
    {
        vecino_izquierdo = ((myrank + k - 1) % k) + k;
        vecino_derecho = ((myrank + k + 1) % k) + k;
        vecino_arriba = myrank - k;
    }
    if (myrank > k)
    {
        x = myrank / k;
        vecino_izquierdo = ((myrank + k - 1) % k) + x * k;
        vecino_derecho = ((myrank + k + 1) % k) + x * k;
        vecino_arriba = myrank - k;
    }
    if (myrank == 0 || (myrank / k) < (k - 1))
    {
        vecino_abajo = myrank + k;
    }
    if ((myrank / k) == (k - 1))
    {
        vecino_abajo = myrank - ((k - 1)*k);
    }
    x = 0;

    for(int kk = 0; kk < PP; kk++) //algorithm
    {
        for (i = 0; i < n / PP; i++)
        {
            for (j = 0; j < n / PP; j++)
            {
                for (k = 0; k < n / PP; k++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
      }

    MPI_Gather (C[from], n*n/P, MPI_INT, C, n*n/P, MPI_INT, 0, MPI_COMM_WORLD);

    //printf("Tiempo transcurrido: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
    average += ((double)clock() - start) / CLOCKS_PER_SEC; 
    
  }
  average = (average/100);
  //printf("Promedio: %f",average);
  MPI_Finalize();
  printf("Promedio: %f\n",(average));


  //Main plot
    int pid, statusz;
    float f[MAXL] = {0.0};
    char *fn = argc > 1 ? argv[1] : "gnuplot.dat";
    char fnbase[MAXC] = "", fnplt[MAXC] = "";
    size_t i;
    FILE *fp = NULL;

    for (i = 0; i < MAXL; i++)      /* fill array of values   */
        f[i] = average;//average;  

    fp = xfopen (fn, "w");      /* open output file */

    for (i = 0; i < MAXL; i++)  /* write values to file */
        fprintf (fp, "%3zu %10.6f\n", i, f[i]);

    xfclose (fp);   /* close output file */

    /* create 'plot' file 'fn.plt' */
    strcpy (fnplt, fnwoext (fnbase, fn));
    strcat (fnplt, ".plt");
    if (!xfexists (fnplt)) {
        xfopen (fnplt, "w");
        fprintf (fp, "set xlabel 'Numero de nucleos'\n"
                    "set ylabel 'Tiempo de procesamiento'\n"
                    "set title 'Grafica de procesadores vs tiempo'\n"
                    "set grid\n"
                    "set style data lines\n"
                    "plot \"%s\" using 1:2 lw 3 linecolor rgb \"blue\"\n"
                    "quit\n", fn);
        xfclose (fp);
    }

    /* fill arguments array for execvp */
    char *args[] = { "gnuplot", "-p", fnplt, NULL };

    if ((pid = (fork())) < 0) { /* fork plot process */
        fprintf (stderr, "fork() error: fork failed.\n");
        return 1;
    }
    else if (pid == 0) {    /* plot from child process */
        if (execvp (*args, args) == -1) {
            fprintf (stderr, "execvp() error: returned error.\n");
            _exit (EXIT_FAILURE);
        }
    }

    waitpid (pid, &statusz, 0);  /* wait for plot completion (not req'd) */


  return 0;
}

//PLOT STUFF

/** fopen with error checking - short version */
FILE *xfopen (const char *fn, const char *mode)
{
    if (!fn || !mode || badmode (mode)) {
        fprintf (stderr, "xfopen() error: invalid parameter.\n");
        exit (EXIT_FAILURE);
    }
    FILE *fp = fopen (fn, mode);

    if (!fp) {
        fprintf (stderr, "xfopen() error: file open failed '%s'.\n", fn);
        exit (EXIT_FAILURE);
    }

    return fp;
}

/** validate file mode 's' is "rwa+b" */
int badmode (const char *s)
{
    const char *modes = "rwa+b";

    for (; *s; s++) {
        const char *m = modes;
        int valid = 0;
        while (*m) if (*s == *m++) { valid = 1; break; }
        if (!valid) return *s;
    }
    return 0;
}

/** file close with error check */
int xfclose (FILE *fp)
{
    if (fclose (fp)) {
        fprintf (stderr, "xfclose() error: nonzero return on fclose.\n");
        return 1;
    }
    return 0;
}

/** check if file 'fn' already exists */
int xfexists (char *fn)
{
    /* if access return is not -1 file exists */
    if (access (fn, F_OK ) != -1 )
        return 1;

    return 0;
}

/** isolate filename, without path or extension */
char *fnwoext (char *nm, char *fn)
{
    char *p  = NULL, *ep = NULL;
    char fnm[MAXC] = "";

    if (!fn) return NULL;
    strcpy (fnm, fn);
    if ((p = strrchr (fnm, '/')))
        p++;
    else
        p = fnm;

    if ((ep = strrchr (p, '.'))) {
        *ep = 0;
        strcpy (nm, p);
        *ep = '.';
    } else
        strcpy (nm, p);

    return nm;
}

