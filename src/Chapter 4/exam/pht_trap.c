#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

// Compile:  gcc -g -Wall -o test pht_trap.c -lpthread


int thread_count;
double GLOBAL_MUTEX_SUM = 0;
pthread_mutex_t mutex;


double time_spent;
double serial_time_spent;


double a;        
double b;         
long int n;       
double h;         

double f(double x);
void* mutex_trapezoid(void* rank);
void mutex_main();
#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)  (BLOCK_LOW((id)+1,p,n)-1)

int main( int argc, char **argv )
{
  mutex_main();

  return 0;
}



void* mutex_trapezoid(void* rank)
{ 
  long thread_rank = (long)rank;
  double local_sum = 0.0;
  long long i;
  int special_case = (int)thread_rank;

  long long my_first_i = BLOCK_LOW(thread_rank, thread_count, n);
  long long my_last_i = BLOCK_HIGH(thread_rank, thread_count, n);

  
  if( special_case == 1)
  {
    local_sum += (f(a)+f(b))/2.0;
  }

  for( i= my_first_i; i <= my_last_i; i++)
  {
    local_sum += f(a+(i*h));
  }
  local_sum = local_sum * h;


  pthread_mutex_lock(&mutex);
  GLOBAL_MUTEX_SUM += local_sum;
  pthread_mutex_unlock(&mutex);

  return NULL;
 
}

void mutex_main()
{              // set time to 0
  pthread_t* thread_handles;

  GLOBAL_MUTEX_SUM = 0;
  pthread_mutex_init(&mutex, NULL);
  thread_handles = malloc( thread_count * sizeof(pthread_t));


  long thread;
  for( thread=0; thread < thread_count; thread++)
    pthread_create( &thread_handles[thread], NULL, mutex_trapezoid, 
      (void*) thread );


  for( thread=0; thread < thread_count; thread++)
    pthread_join( thread_handles[thread], NULL);

  free(thread_handles);
  pthread_mutex_destroy(&mutex);
 }

double f(double x)
{
  double return_val = x*x;
  return return_val;
}

