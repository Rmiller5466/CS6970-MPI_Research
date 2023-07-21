#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <malloc.h>
#include <time.h>

int main(int argc, char *argv[])
{
        int my_rank, world_size;

        MPI_Comm intercomm;

        char nodename[BUFSIZ];
        int nodename_length;

        int numCpus = atoi(argv[1]);
        // int bound = atoi(argv[2]);

        char *child_args[] = {argv[2], NULL};
        char *child_path = "/home/w051rem/IndependentStudy/Final_Tests/MPIExecForkCommSpawn/Child.mpi";

        (void)MPI_Init(&argc, &argv);
        (void)MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        (void)MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        (void)MPI_Get_processor_name(nodename, &nodename_length);

        printf("%s - Parent[%d]: Hello\n", nodename, my_rank);

        MPI_Comm_spawn(child_path, child_args, numCpus,
                       MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm,
                       MPI_ERRCODES_IGNORE);

        MPI_Finalize();
}
