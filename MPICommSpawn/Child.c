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

        (void)MPI_Init(&argc, &argv);
        (void)MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        (void)MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        (void)MPI_Get_processor_name(nodename, &nodename_length);

        printf("%s - Child[%d]: Hello\n", nodename, my_rank);

        long double the_num = 0;
        long double my_num = 0;
        int bound = atoi(argv[1]);

        time_t begin = time(NULL);
        time_t end;

        if (my_rank == 0)
        {
                // Rank is 0, wait for other processes to send their calculated numbers
                MPI_Reduce(&my_num, &the_num, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

                // Timing for how long it takes for all processes to report back to Rank 0
                end = time(NULL);
        }
        else
        {
                // Each rank does its own range of numbers determined by the variable, bound
                // For loop counts DOWN from my_start to my_end
                /*
                Example: Total Ranks = 3, bound = 100
                        my_rank = 0
                        my_start = 0
                        my_end = 0

                        my_rank = 1
                        my_start = 100
                        my_end = 0

                        my_rank = 2
                        my_start = 200
                        my_end = 100
                */

                int my_start = my_rank * bound;
                int my_end = my_start - bound;

                for (int i = my_start; i > my_end; i--)
                {
                        // Random mathematical formula to imitate ranks doing work
                        my_num += (log10(cbrt(i) + (0.25 * cbrt(i))) / log10(bound)) * 0.1;

                        // For quick testing we can use the following simple operation instead
                        // my_num += i;
                }

                end = time(NULL);
                printf("%s - Child[%d]: Calculated %Lf\n", nodename, my_rank, my_num);
                MPI_Reduce(&my_num, &the_num, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        }

        double time_spent = (end - begin);

        (void)MPI_Barrier(MPI_COMM_WORLD);

        if (my_rank == 0)
        {
                printf("\n--------------------\n%s - Child[0]: %f seconds for children in this group to finish\n%s - Child[0]: Children in this group calculated %Lf\n--------------------\n\n", nodename, time_spent, nodename, the_num);
        }
        else
        {
                printf("%s - Child[%d]: %f seconds to execute\n", nodename, my_rank, time_spent);
        }

        MPI_Finalize();
}
