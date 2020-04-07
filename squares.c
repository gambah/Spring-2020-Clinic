//this code can send an array through MPI broadcast and it can time the entire proccess 

#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

//Sets up a my_bcast variable that can be called in the main loop.
void my_bcast(void* array, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if (world_rank == root) {
    //Declares that the root (master) sends info to the slaves
    int i;
    for (i = 0; i < world_size; i++) {
      if (i != world_rank) {
        MPI_Send(array, count, datatype, i, 0, communicator);
      }
    }
  } 
  else {
    //Recieves info that is broadcasted
    MPI_Recv(array, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
  }
}

int main(int argc, char** argv){

	MPI_Init(&argc, &argv);
 
  // time
  double start, end;

  MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
  start = MPI_Wtime();
  
	//Declares world size and processor rank& name.
	int my_rank, world_size, x;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);
 
  x = 10;
  int n[x];
  int count;
  for(count = 0; count < x; count++) { 
    n[count] = count + 1; 
	}
  
	//How many squares that need to be found.
	if(my_rank == 0){

		my_bcast(&n, x, MPI_INT, 0, MPI_COMM_WORLD);
	}
  else{
		my_bcast(&n, x, MPI_INT, 0, MPI_COMM_WORLD);	
	}

	//tells how many times the number of squares needed goes into the amount of processors that are available.
	double y, i, b;
	y = x / world_size;
 
  for(count = 0; count < x; count++) { 
    n[count] = count + 1; 
	}

	//Makes the output read "1 of 4", "2 of 4", "3 of 4", "4 of 4", instead of "0 of 4", etc.
	int fart;
	fart = my_rank + 1;
	
	for(i=(y*my_rank)+1; i<=y*(my_rank+1); i++)
	{	
		f = (100000*i*i*i*i*i*i*i)+(i*i*i*i*i*i)-(1254364*i*i*i*i*i)-(i*i)+16543;
		g = (120*i*i);
		h = (2789*i*i*i*i*i)-(1234432*i*i*i)-(i*i);
		b = (156825*i*i*i*i)-(412343*i*i*i)+(18*i*i)-(32*i)-64;
		printf("b = %f. Printed by processor %d of %d\n",b,fart,world_size);
	}
	
  for(int arrayspot = 0; arrayspot < x; arrayspot++) { 
    printf("%d\n", n[arrayspot]); 
	} 
 
  MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
  end = MPI_Wtime();

	MPI_Finalize();
 
  if (my_rank == 0) { /* use time on master node */
    printf("Runtime = %f\n", end-start);
  }
}
