#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

typedef struct buffer_s {
        int   locate;             /*生产者的index*/
        int  array[20]; /*用于保存产品*/
} buffer;
buffer*p;

double expntl(double x) 
{ 
  double z;                     // Uniform random number from 0 to 1 
 
  // Pull a uniform RV (0 < z < 1) 
  do 
  { 
    z = ((double) rand() / RAND_MAX); 
  } 
  while ((z == 0) || (z == 1)); 
 
  return(-x * log(z)); 
} 