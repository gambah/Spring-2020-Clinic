//This code can populate values into an array, send the array, square each value within the array, and time the entire process. 

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
 
  //Creates and populates an array with values 1 through x.
  x = 100;
  int n[x];
  int count;
  for(count = 0; count < x; count++) { 
    n[count] = count + 1; 
	}
  
	//Broadcasts the array.
	if(my_rank == 0){
		my_bcast(&n, x, MPI_INT, 0, MPI_COMM_WORLD);
	}
  else{
		my_bcast(&n, x, MPI_INT, 0, MPI_COMM_WORLD);	
	}

	//Tells how many times the number of squares needed goes into the amount of processors that are available.
	double y;
  int i, b;
	y = x / world_size;

	//Makes the output read "1 of 4", "2 of 4", "3 of 4", "4 of 4", instead of "0 of 4", etc.
	int fart;
	fart = my_rank + 1;
	
	//Squares each value within the array. 
	for(i=(y*my_rank)+1; i<=y*(my_rank+1); i++)
	{	
		b = (n[i-1]*n[i-1]);
		printf("b = %d. Printed by processor %d of %d\n",b,fart,world_size);
	}
 
  MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
  end = MPI_Wtime();

	MPI_Finalize();
 
  if (my_rank == 0) { /* use time on master node */
    printf("Runtime = %f\n", end-start);
  }
}
