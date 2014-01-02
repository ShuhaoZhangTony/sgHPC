#!/bin/bash
echo "compile File"
#SBATCH -p gpu
#SBATCH -n 12
#SBATCH -N 2
#SBATCh -ntasks-per-node=2

module load gcc/4.7.0
module load openmpi/gcc

	if [ -f a.out ]		
	then				
		mpirun a.out 1024*1024
    	else
		echo "File not exist."
	fi

