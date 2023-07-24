# CS-6970: Independent Study in Computer Science
- Subject: Dynamic MPI
- Advisor: Dr. John Nehrbass
- Student: Ryan Miller

## Introduction and Project Description

The Message Passing Interface (MPI) standard is the dominant communication protocol for programming parallel applications on High Performance Computing (HPC) systems.  Features for dynamic process management were introduced in MPI-2, centering around the ability to create, connect, and communicate between separate MPI processes.  Unfortunately, many of these features have undefined functionality or lack of support on many resource  scheduling systems.  Specifically, this project explored techniques for launching a pre-compiled, source unavailable MPI code from a parent MPI process while utilizing only the resources allocated to the parent by a job queuing system.      

## Systems, Software and Limitations

This project explored the MPICH and OpenMPI implementations of the MPI standard.  Slurm was also exclusively used as the job-scheduling system. The specific versions used are:

- MPICH: 3.3.2_gcc-8.50
- OpenMPI: 4.0.3_gcc-8.50
- Slurm 22.05.2

This study was only able to be preformed on Wright State's HPC cluster, Fry, and with no ability to explore how different build configuration parameters for Slurm or MPI distributions may affect the ability to accomplish the goal.  Due to this limitation, the study only reflects the above software versions as well as any limitations introduced by the configuration of Wright State's cluster.  

## Proposed Solutions

This study found 3 potential solutions to the problem statement.  Solution 1 works for both the MPICH and Open-MPI distributions.  Solutions 2 and 3 utilize additional launch parameters provided by the Hydra process manager, and were only able to be shown working using MPICH.  These solutions may be applicable to Open-MPI, though specifics on system configuration are unknown. 

Throughout testing, it was observed that the MPICH distribution of MPI would not verify whether enough resources were available to spawn the desired number of children.  When attempting to spawn more children than resources available, the system would oversubscribe the allocated resources.  When using any of these techniques, the user must be mindful of allocated resources, otherwise issues may arise.  

### [1. Launching Children Processes With a New Shell and Cleared Environment](./ClearedShell)

The first method for accomplishing the desired functionality involves using a system call with an empty environment from within the parent process. The system call executes the following command:

```C
"sh -c 'env -i PATH=/opt/apps/mpi/mpich-3.3.2_gcc-8.5.0/bin/:/bin mpirun -np NCPUS ./Child.mpi BOUND'"
```

In the example given, the only PATH variable set is to the MPI version being used, though depending on system configuration additional PATH variables may need set in addition to the MPI location.

The Open-MPI distribution was observed to properly detect and abort the program execution when using more resources than were available.  

### [2. Launching Children Processes By Changing Process Launcher](./InternalFork)

The second method found still involves using a system call, though changes the mpiexec -launcher parameter.  The system call made executes the following command: 

```C
"mpiexec -launcher ssh -np %d ./Child.mpi %d"
```

This technique was found to work best with an -launcher argument of 'ssh', though the -launcher flag allows values of: ssh, rsh, pdsh, pbs, fork, persist, slurm, ll, lsf, and sge.

Fork was the only other value tested with -launcher, though using fork to launch the children MPI processes led to communication errors and program crashes when more than one node was used.  

### [3. Built In MPI method: MPI_Comm_spawn](./MPICommSpawn)

The third method found involves the "official" way to meet the goal of this project.  MPI_Comm_spawn is a built in MPI method that will spawn a set of children processes when called from within an MPI program.  While there are methods that can interface with the set of spawned children, these were not explored due to the constraint that the source code of the children processes is not available.  The call to MPI_Comm_spawn done within the parent program is shown:

```C
MPI_Comm_spawn(child_path, child_args, numCpus,
                       MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm,
                       MPI_ERRCODES_IGNORE);
```

Additionally, to be able to run this within the Slurm system the following modification needed made to the batch script, changing the -launcher option on the parent process:

```C
mpirun -launcher ssh -np 1 ./Parent.mpi ncpus bound
```

While this example utilizes MPICH to access the -launcher option, the official Slurm documentation states the following in reference to MPI_Comm_spawn and the OpenMPI distribution:

"NOTE: OpenMPI has a limitation that does not support calls to MPI_Comm_spawn() from within a Slurm allocation. If you need to use the MPI_Comm_spawn() function you will need to use another MPI implementation combined with PMI-2 since PMIx doesn't support it either."

For more technical details on the syntax of any of the preceding techniques, please refer to the provided code which includes the Slurm batch scripts for resource allocation and full Parent / Child MPI program code.  

## Future Research
- Does an alternative to "-launcher ssh" exist for the Open-MPI distribution?
- How do these solutions hold up on scheduling systems that are not Slurm?
- Is there a way simplify this problem by making changes to the system-wide build and configuration of MPICH, OpenMPI, or Slurm?
- How would the use of hostfiles/machinefiles affect these solutions? 

## References
- Documentation
	- https://slurm.schedmd.com/mpi_guide.html
	- https://www.intel.com/content/www/us/en/docs/mpi-library/developer-reference-linux/2021-8/global-hydra-options.html
	- https://www.mpich.org/static/docs/v4.1.x/
	- https://www-lb.open-mpi.org/doc/current/
- Discussions / Forums 
	- https://www.mail-archive.com/users@lists.open-mpi.org/msg34830.html
	- https://stackoverflow.com/questions/21497605/creating-child-process-with-mpi-comm-spawn?rq=4
	- https://slurm-dev.schedmd.narkive.com/kJbLUdc4/mpi-comm-spawn-with-mpich2-via-slurm-pm  
	- https://www.mpi-forum.org/docs/mpi-2.2/mpi22-report/node207.htm
	- https://www.mpi-forum.org/docs/mpi-2.2/mpi22-report/node208.htm
	- https://users.open-mpi.narkive.com/ywODa41n/ompi-mpi-inside-mpi-still
	- https://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-2.0/node98.htm
	- https://lists.mpich.org/pipermail/discuss/2021-November/006278.html
	- https://slurm-dev.schedmd.narkive.com/kJbLUdc4/mpi-comm-spawn-with-mpich2-via-slurm-pm
	- https://lists.mpich.org/pipermail/discuss/2022-January/006332.html
	- https://lists.mpich.org/pipermail/discuss/2022-February/006391.html
	- https://github.com/pmodels/mpich/issues/5835
	- https://stackoverflow.com/questions/17950762/block-execution-until-children-called-via-mpi-comm-spawn-have-finished
	- https://en.wikipedia.org/w/index.php?title=Message_Passing_Interface#Dynamic_process_management
