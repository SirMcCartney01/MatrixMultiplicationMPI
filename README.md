# MatrixMultiplicationMPI
This program is part of the Operative Systems II course evaluation

The MatrixMultiplicationMPI is a project that creates a matrix of random numbers between 1k and 2k, there're different sizes, such as:
* 16
* 32
* 64
* 128
* 256
* 512
* 1024
* 2048

And diferent data types:
* int
* float

Algorithms:
* Secuential
* Cannon

Also, we have two ways for multiplying the matrix, we use the secuencial algorithm which multiplies the matrix in the usual way or we can
use Cannon's algorithm tho optimize the the performance (or so we thought).

All this implemented with parallelism using the OpenMPI library for C, so we can run out test with the following number of cores:
* 1
* 2
* 4
* 8

Finally, for the output, we need to get the graphs of the time vs number of cores and memory vs number of cores, these two graphs for each
matrix size and each data type.

Version 1.0.
* Implemented MPI.
* Implemented Secuential algorithm.
* Implemented Cannon's algorithm.
* Created Java GUI.
* Created graphs (although it's not working properly).

Upcomming features:
* Create a JNI to connect the Java GUI with the C code.
* Automate the selection of matrix size, data type and number of cores (GUI).
* Fix graphs bugs.

Cesar Placido.
