/*************************************************************************//**

 *    gcc -g -Wall -o trapezoid trapezoid.c -lpthread

 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// parallel specific variables
const long thread_count = 8;
double GLOBAL_MUTEX_SUM = 0;
double GLOBAL_SEM_SUM = 0;
double GLOBAL_BUSY_SUM = 0;
pthread_mutex_t mutex;
sem_t semaphore;
int wait_flag = 0;

// trapezoid globals
double a;         // left endpoint
double b;         // right endpoint
long int n;       // number of trapezoids
double h;         // height of trapezoids

double f(double x);
double serial_trapezoid();
void* busy_wait_trapezoid(void* rank);
void* mutex_trapezoid(void* rank);
void* sem_trapezoid(void* rank);
void mutex_main();
void sem_main();
void busy_wait_main();

// MACROS provided by Dr. Karlsson for distributing work to threads
#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)  (BLOCK_LOW((id)+1,p,n)-1)

int main( int argc, char **argv )
{
  ///////////////////////////////  SERIAL /////////////////////////////////////
  double integral;  // store result

  printf("Enter a, b, and n\n");
  scanf("%lf",&a);
  scanf("%lf",&b);
  scanf("%ld",&n);

  h = (b-a)/n;
  integral = serial_trapezoid();

  printf("With n = %ld trapezoids, our estimate\n", n);
  printf("of the integral from %f to %f = %.15f\n\n",
    a, b, integral);
//////////////////////////////   /SERIAL //////////////////////////////////////

  mutex_main();
  return 0;
}

// This function uses code from trap.c provided on Dr. Karlsson's website. The
// trapezoidal rule is used to calculate the integral of f(x) with endpoints a
// and b, and using n trapezoids. This is done on one thread.
double serial_trapezoid()
{
  double integral;
  int i;

  integral = (f(a)+f(b))/2.0;

  for( i=1; i <= n-1; i++)
  {
    integral += f(a+(i*h));
  }
  integral = integral*h;

  return integral;

}

// This function calculates the integral using the trapezoidal rule in
// parallel. The calculation used here is the same as in serial_trapezoid()
// except (f(a)+f(b))/2.0 needed to be special cased -> it's only added once.
// A spinlock is used to address the problems that could occur from race
// conditions where two or more threads are trying to add to the global
// sum at once.
void* busy_wait_trapezoid(void* rank)
{

  long thread_rank = (long)rank;
  double local_sum = 0.0;	// thread specific sum
  long long i;
  int special_case = (int)thread_rank;

  // allocate a chunk of work to the thread
  long long my_first_i = BLOCK_LOW(thread_rank, thread_count, n);
  long long my_last_i = BLOCK_HIGH(thread_rank, thread_count, n);

  // let thread with rank 1 add (f(a)+f(b))/2.0 to it's sum. This is only
  // done once for trapezoidal rule calculation & thread that does it is
  // should be 1st or 2nd in case code is only run on 2 cores.
  if( special_case == 1)
  {
    local_sum += (f(a)+f(b))/2.0;
  }
  // Karlsson's trapezoid code
  for( i= my_first_i; i <= my_last_i; i++)
  {
    local_sum += f(a+(i*h));
  }
  local_sum = local_sum * h;

  // update the global sum
  while( wait_flag != thread_rank );
  GLOBAL_BUSY_SUM += local_sum;
  wait_flag = (wait_flag+1) % thread_count;

  return NULL;
}


void* mutex_trapezoid(void* rank)
{ 
  long thread_rank = (long)rank;
  double local_sum = 0.0;
  long long i;
  int special_case = (int)thread_rank;

  // allocate a chunk of work to the thread
  long long my_first_i = BLOCK_LOW(thread_rank, thread_count, n);
  long long my_last_i = BLOCK_HIGH(thread_rank, thread_count, n);

  // let thread with rank 1 add (f(a)+f(b))/2.0 to it's sum. This is only
  // done once for trapezoidal rule calculation & thread that does it is
  // should be 1st or 2nd in case code is only run on 2 cores.
  if( special_case == 1)
  {
    local_sum += (f(a)+f(b))/2.0;
  }

  // Karlsson's trapezoid code
  for( i= my_first_i; i <= my_last_i; i++)
  {
    local_sum += f(a+(i*h));
  }
  local_sum = local_sum * h;

  // update the global sum
  pthread_mutex_lock(&mutex);
  GLOBAL_MUTEX_SUM += local_sum;
  pthread_mutex_unlock(&mutex);

  return NULL;
 
}


void* sem_trapezoid(void *rank)
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

  // update the global sum
  sem_wait(&semaphore);
  GLOBAL_SEM_SUM += local_sum;
  sem_post(&semaphore);

  return NULL;
}

void mutex_main()
{
  pthread_t* thread_handles;

    // create thread handles and initialize mutex
  pthread_mutex_init(&mutex, NULL);
  thread_handles = malloc( thread_count * sizeof(pthread_t));

  // create the pthreads on mutex trapezoid function
  long thread;
  for( thread=0; thread < thread_count; thread++)
    pthread_create( &thread_handles[thread], NULL, mutex_trapezoid, 
      (void*) thread );

  // join all the thread handles
  for( thread=0; thread < thread_count; thread++)
    pthread_join( thread_handles[thread], NULL);

  // free thread handles and mutex
  free(thread_handles);
  pthread_mutex_destroy(&mutex);

  printf("MUTEX\n");
  printf("With n = %ld trapezoids, our estimate\n", n);
  printf("of the integral from %f to %f = %.15f\n\n",
    a, b, GLOBAL_MUTEX_SUM);  
}

double f(double x)
{
  double return_val = x*x;
  return return_val;
}
