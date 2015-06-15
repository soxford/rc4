/*
 * Author: Joe Pitt-Francis
 * Date: 15/07/2002
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int 
main(int argc, char *argv[])
{
  int my_id, left_id, right_id, numprocs;
  char send_string[10], recv_string[10];
  MPI_Status status;
  int ierr;

  /*Initialize MPI*/
  MPI_Init(&argc, &argv);
  
  /*Work out how this process fits with the others*/
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_id);

  /*Work out who's to the left (in a ring)*/
  left_id=my_id-1;
  if (my_id==0) left_id=numprocs-1; 
  
  /*Work out who's to the right (in a ring)*/
  right_id=my_id+1;
  if (my_id==numprocs-1) right_id=0;

  /*Form a message*/
  sprintf(send_string, "I am %i", my_id);

  /*Send a message to the right (receive from the left)*/
  ierr=MPI_Sendrecv(&send_string, 10, MPI_CHAR, right_id, 0,
		    &recv_string, 10, MPI_CHAR,  left_id, 0,
		    MPI_COMM_WORLD, &status);

  if (ierr!=MPI_SUCCESS){
    printf("There was an error\n");
  } else {
    printf("Process %i received <%s> from process %i\n", 
	   my_id, recv_string, left_id);
  }
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}
