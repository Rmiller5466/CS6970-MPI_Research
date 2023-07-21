#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <malloc.h>
#include <time.h>

int main(int argc, char *argv[])
{
        int my_rank, world_size;
        char nodename[BUFSIZ];
        int nodename_length;

        // Number of children mpi processes to spawn
        int numCpus = atoi(argv[1]);

        // Parameter for calculations within Child.mpi
        int bound = atoi(argv[2]);

        (void)MPI_Init(&argc, &argv);
        (void)MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        (void)MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        (void)MPI_Get_processor_name(nodename, &nodename_length);

        printf("%s - Parent[%d]: Hello\n", nodename, my_rank);

        char buf[1024];

        snprintf(buf, sizeof(buf), "mpiexec -launcher ssh -np %d ./Child.mpi %d", numCpus, bound);

        printf("%s - Parent [%d]: Running command: %s\n", nodename, my_rank, buf);

        // Timing for how long it takes for the system command to return
        time_t begin = time(NULL);

        // If the system call is carried out without error, a system code of 0 will be returned
        int sysCode = system(buf);

        time_t end = time(NULL);

        double time_spent = (end - begin);

        printf("\n====================\n%s - Parent[%d]: %f seconds for children to finish.  System Code - %d\n====================\n\n", nodename, my_rank, time_spent, sysCode);

        (void)MPI_Barrier(MPI_COMM_WORLD);
        MPI_Finalize();
}
