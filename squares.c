#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

//Sets up a my_bcast variable that can be called in the main loop.
void my_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if (world_rank == root) {
    //Declares that the root (master) sends info to the slaves
    int i;
    for (i = 0; i < world_size; i++) {
      if (i != world_rank) {
        MPI_Send(data, count, datatype, i, 0, communicator);
      }
    }
  } else {
    //Recieves info that is broadcasted
    MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
  }
}

int main(int argc, char** argv){

	MPI_Init(NULL,NULL);
	
	//initializes the timer and starts it.
	double t1, t2, calc_time;
	 
	//Declares world size and processor rank& name.
	int my_rank, world_size, n;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);
	
	//How many squares that need to be found.
	if(my_rank == 0){
		n=10000;
		my_bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	}else{
		my_bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	}

	//tells how many times the number of squares needed goes into the amount of processors that are available.
	double y, i, b, f, g, h;
	y = n / world_size;

	//Makes the output read "1 of 4", "2 of 4", "3 of 4", "4 of 4", instead of "0 of 4", etc.
	int fart;
	fart = my_rank + 1;
	
	//Begins timer
	t1 = MPI_Wtime();
	
	//does the thing.
	for(i=(y*my_rank)+1; i<=y*(my_rank+1); i++)
	{	
		f = (100000*i*i*i*i*i*i*i)+(i*i*i*i*i*i)-(1254364*i*i*i*i*i)-(i*i)+16543;
		g = (120*i*i);
		h = (2789*i*i*i*i*i)-(1234432*i*i*i)-(i*i);
		b = (156825*i*i*i*i)-(412343*i*i*i)+(18*i*i)-(32*i)-64;
		printf("b = %f. Printed by processor %d of %d\n",b,fart,world_size);
	}
	
	//Ends timer
	t2 = MPI_Wtime();

	//Calculates time
	calc_time = t2 -t1;

	printf("The time for me to find the answer was %f s. Warm Regards, Processor %d of %d\n",calc_time,my_rank,world_size);

	MPI_Finalize();
} 
