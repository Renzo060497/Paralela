#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]) {

	int rank_sz, rank_x, rank_y, my_rank;
	long **A, *x, *y, *miFila, *comprueba;
	double tInicio, tFin;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &rank_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	A = new long*[rank_sz];
	x = new long[rank_sz];

	if (my_rank == 0) {
		printf("El numero de columnas es: %d\n", rank_sz);
		printf("Digite el numero de filas: \n");
		scanf("%d", &rank_y);
		A[0] = new long[rank_sz * rank_y];
		for (unsigned int i = 1; i < rank_sz; i++) {
			A[i] = A[i - 1] + rank_sz;
		}
		y = new long[rank_sz];

		srand(time(0));
		cout << "\nLa matriz generada es: " << endl;
		for (unsigned int i = 0; i < rank_y; i++) {
			for (unsigned int j = 0; j < rank_sz; j++) {
				if (j == 0)
					cout << "[";
				A[i][j] = rand() % 1000;
				cout << A[i][j];
				if (j == rank_sz - 1)
					cout << "]\n";
				else
					cout << "  ";
			}
		}

		cout << "\nEl vector generado es: " << endl;
		for (unsigned int k = 0; k < rank_sz; k++) {
			x[k] = rand() % 100;
			cout << "\t  [" << x[k] << "]" << endl;
		}

		cout << "\n";

		comprueba = new long[rank_sz];
		for (unsigned int i = 0; i < rank_sz; i++) {
			comprueba[i] = 0;
			for (unsigned int j = 0; j < rank_sz; j++) {
				comprueba[i] += A[i][j] * x[j];
			}
		}
	}

	miFila = new long[rank_sz];

	MPI_Scatter(A[0], rank_sz, MPI_LONG, miFila, rank_sz, MPI_LONG, 0, MPI_COMM_WORLD);
	MPI_Bcast(x, rank_sz, MPI_LONG, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	tInicio = MPI_Wtime();

	long subFinal = 0;
	for (unsigned int i = 0; i < rank_sz; i++) {
		subFinal += miFila[i] * x[i];
	}

	MPI_Barrier(MPI_COMM_WORLD);
	tFin = MPI_Wtime();
	MPI_Gather(&subFinal, 1, MPI_LONG, y, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	MPI_Finalize();

	if (my_rank == 0) {
		unsigned int errores = 0;
		cout << "El resultado obtenido y el esperado son:" << endl;
		for (unsigned int i = 0; i < rank_y; i++) {
			cout << "\t" << y[i] << "\t|\t" << comprueba[i] << endl;
			if (comprueba[i] != y[i])
				errores++;
		}

		delete[] y;
		delete[] comprueba;
		delete[] A[0];

		if (errores) {
			cout << "Hubo " << errores << " errores." << endl;
		} else {
			cout << "No hubo errores" << endl;
			cout << "El tiempo tardado ha sido " << tFin - tInicio << " segundos." << endl;
		}
	}
	delete[] x;
	delete[] A;
	delete[] miFila;
}
