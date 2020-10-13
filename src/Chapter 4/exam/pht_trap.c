/*************************************************************************//**
 *    gcc -g -Wall -o pht_trap pht_trap.c -lpthread
 *****************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

// Compile:  gcc -g -Wall -o trapezoid_timed trapezoid_timed.c -lpthread


int thread_count;
double GLOBAL_MUTEX_SUM = 0;
pthread_mutex_t mutex;

// global time variables to store amount of time spent computing integral
double time_spent;
double serial_time_spent;

// trapezoid globals
double a;         // left endpoint
double b;         // right endpoint
long int n;       // number of trapezoids
double h;         // height of trapezoids

double f(double x);
double serial_trapezoid();
void* mutex_trapezoid(void* rank);
void mutex_main();
#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)  (BLOCK_LOW((id)+1,p,n)-1)

int main( int argc, char **argv )
{
  ///////////////////////////////  SERIAL /////////////////////////////////////
  double integral;  // store result
  struct timeval begin, end;
  time_spent = 0;
  serial_time_spent = 0;

  printf("Enter a (left endpt), b (right endpt), and n (# trapezoids)\n");
  scanf("%lf",&a);
  scanf("%lf",&b);
  scanf("%ld",&n);
  h = (b-a)/n;		// calculate height

  gettimeofday(&begin, NULL);
  integral = serial_trapezoid();
  gettimeofday(&end, NULL);
  serial_time_spent = (double)((end.tv_sec-begin.tv_sec) + 
    (end.tv_usec-begin.tv_usec)/1000000.0);

  printf("\n----- 1 thread -----");
  printf("\nWith n = %ld trapezoids, our estimate\n", n);
  printf("of the integral from %f to %f = %.15f\n",
    a, b, integral);
  printf("The elapsed time with 1 processor is %e seconds\n\n", 
        serial_time_spent);
//////////////////////////////   /SERIAL ////////////////////////////////////// 

  // call mutex trapezoidal funcs with 2, 4, and 8 threads
  thread_count = 2;
  mutex_main();
  thread_count = 4;
  mutex_main();
  thread_count = 8;
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
// A shared mutex is used to address the problems that could occur from race
// conditions where two or more threads are trying to add to the global
// sum at once.
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

// This function creates thread_count(specified in main) pthreads
// and initializes a mutex that will be used for the global sum in the
// mutex_trapezoid() function. The threads are joined after finishing
// execution, mem assoc. w/ threads is freed, and mutex destroyed.
// The integral estimation is printed as well.
void mutex_main()
{
  struct timeval begin, end;    // init time struct
  time_spent = 0;               // set time to 0
  pthread_t* thread_handles;

  GLOBAL_MUTEX_SUM = 0;

  // create thread handles and initialize mutex
  pthread_mutex_init(&mutex, NULL);
  thread_handles = malloc( thread_count * sizeof(pthread_t));

  // spawn and START timing of pthreads /////////////////////
  gettimeofday(&begin, NULL);
  // create the pthreads on mutex trapezoid function
  long thread;
  for( thread=0; thread < thread_count; thread++)
    pthread_create( &thread_handles[thread], NULL, mutex_trapezoid, 
      (void*) thread );

  // join all the thread handles
  for( thread=0; thread < thread_count; thread++)
    pthread_join( thread_handles[thread], NULL);

    gettimeofday(&end, NULL);
  time_spent = (double)((end.tv_sec-begin.tv_sec) + (end.tv_usec-begin.tv_usec)/1000000.0);
  // END timing of pthreads ////////////////////////////////

  // free thread handles and mutex
  free(thread_handles);
  pthread_mutex_destroy(&mutex);
 
  printf("----- %d threads -----\n", thread_count);
  printf("With n = %ld trapezoids, our estimate\n", n);
  printf("of the integral from %f to %f = %.15f\n",
    a, b, GLOBAL_MUTEX_SUM);  

  printf("The elapsed time with %d processors is %e seconds\n",thread_count, time_spent);
  double speedup = (double)(serial_time_spent/time_spent);
  printf("Speedup(S) = %lf\n", speedup);
  double efficiency = (double)(serial_time_spent/(time_spent*thread_count));
  printf("Efficiency(E) = %.4lf\n\n", efficiency);
}

// This is the function that is called in the serial and parallel trapezoidal
// functions.
double f(double x)
{
  double return_val = x*x;
  return return_val;
}

