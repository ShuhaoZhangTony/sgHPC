#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <omp.h>
using namespace std;
//#define Debug
void out_of_ordered(int iam,int np)
{
	 #pragma omp parallel private(iam, np)
          {
                np = omp_get_num_threads();
                iam = omp_get_thread_num();
                printf("Hello from thread %d out of %d\n", iam, np);
          }

}
void in_ordered(int iam,int np)
{
	 #pragma omp parallel private(iam, np)
          {
                np = omp_get_num_threads();
                iam = omp_get_thread_num();
		#pragma omp for schedule(static,1) ordered
		for(int i=0;i<np;i++)
			#pragma omp ordered
	                printf("Hello from thread %d out of %d\n", iam, np);
          }

}
int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage: test {N}\n");
		exit(-1);
	}
	size_t npro = atoi(argv[1]);
	omp_set_num_threads(npro);
	int iam = 0, np = 1;
	cout<<"out_of_ordered:"<<endl;
	out_of_ordered(iam,np);
	cout<<"in_ordered:"<<endl;
	in_ordered(iam,np);
}
