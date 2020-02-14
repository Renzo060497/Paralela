#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;
int cantidad;
int fibonacci = 0;
int numero1 = 0;
int numero2 = 1;

void *Suma(void* rank);

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles;
   printf("Ingresa un numero: ");
   scanf("%d",&cantidad);

   thread_count = strtol(argv[1], NULL, 10);
   thread_handles = malloc (thread_count*sizeof(pthread_t));

   for (thread = 0; thread < thread_count; thread++){
	   pthread_create(&thread_handles[thread], NULL, Suma, (void*) thread);
   }

   for (thread = 0; thread < thread_count; thread++){
	   pthread_join(thread_handles[thread], NULL);
   }

   printf("Resultado: %d", fibonacci);

   free(thread_handles);
   return 0;
}

/*-------------------------------------------------------------------*/
void *Suma(void* rank) {
   long my_rank = (long) rank;
   int local_m = cantidad/thread_count;
   int inicio = my_rank*local_m + 1;
   int fin = (my_rank+1)*local_m;

   for(int i = inicio; i < fin; i++){
	   int suma = numero1 + numero2;
	   fibonacci = suma;
	   numero1 = numero2;
	   numero2 = suma;
   }
   return NULL;
}
