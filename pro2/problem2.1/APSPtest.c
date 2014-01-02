#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include "MatUtil.h"

#define NonAPI
int main(int argc, char *argv[]) {
	if(argc != 2)
	{
		printf("Missing Argument\n");
		exit(-1);
	}
	
	int i, j;
	MPI_Status status;
	int *mat, *ref;
	int P, myrank;
	size_t N = atoi(argv[1]); //matrix size
	struct timeval tv1,tv2;
	MPI_Request sreq,rreq;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	mat=(int*)malloc(sizeof(int)*N*N);//buffer for all nodes
	if(myrank==0){
		printf("input Size is %d; number of process is %d \n",N,P);
		GenMatrix(mat, N);//only set value for p0- as root;
	}
	if(myrank==0)
		gettimeofday(&tv1, NULL);
	MPI_Bcast(mat,N*N,MPI_INT,0,MPI_COMM_WORLD);	
	MPI_Barrier(MPI_COMM_WORLD);
	if(myrank==0){
		gettimeofday(&tv2, NULL);
		printf("Bcast API time = %ld usecs\n", 
				(tv2.tv_sec-tv1.tv_sec)*1000000+tv2.tv_usec-tv1.tv_usec);  
		}
	MPI_Barrier(MPI_COMM_WORLD);
	if(myrank==0)
		gettimeofday(&tv1, NULL);
	if(myrank==0){
		for(i=1;i<P;i++){
		//	MPI_Send(mat,N*N,MPI_INT,i,1,MPI_COMM_WORLD);
			MPI_Isend(mat,N*N,MPI_INT,i,1,MPI_COMM_WORLD,&sreq);	
			MPI_Wait(&sreq,&status);
		}
	}
	else{
	//	MPI_Recv(mat,N*N,MPI_INT,0,1,MPI_COMM_WORLD,&status);
		MPI_Irecv(mat,N*N,MPI_INT,0,1,MPI_COMM_WORLD,&rreq);
		MPI_Wait(&rreq,&status);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(myrank==0){
		gettimeofday(&tv2, NULL);
		printf("Bcast:Isend+Irecv time = %ld usecs\n", 
				(tv2.tv_sec-tv1.tv_sec)*1000000+tv2.tv_usec-tv1.tv_usec);  
	}

	MPI_Finalize();
	return 0;
}
