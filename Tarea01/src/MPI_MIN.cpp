/*
 ============================================================================
 Name        : Tarea01.c
 Author      : Renzo
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
 */
#include <math.h>
#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
	int rank, size, resultado, var;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	var = rank;
	MPI_Reduce(&var, &resultado, 1, MPI_INT, MPI_MIN, 0,MPI_COMM_WORLD);

	if(rank == 0){
		cout << "El mínimo rango es: " << resultado << endl;
	}

	MPI::Finalize();
	return 0;
}

