#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int global_var; 
pthread_mutex_t lock;

void* increment(void* arg) 
{
  
  pthread_mutex_lock(&lock);
  for(int i = 0; i < 2000000; i++)
  {
    global_var += 1;
  }

  pthread_mutex_unlock(&lock);
  return 0;
}

int main() 
{
  pthread_mutex_init(&lock, NULL);

  pthread_t threadIDOne;
  pthread_t threadIDTwo;
  pthread_t threadIDThree;
  pthread_t threadIDFour;
  void* exitStatus;
  int value = 42;

  pthread_create(&threadIDOne, NULL, increment, &value);
  pthread_create(&threadIDTwo, NULL, increment, &value);
  pthread_create(&threadIDThree, NULL, increment, &value);
  pthread_create(&threadIDFour, NULL, increment, &value);

  pthread_join(threadIDOne, &exitStatus);
  pthread_join(threadIDTwo, &exitStatus);
  pthread_join(threadIDThree, &exitStatus);
  pthread_join(threadIDFour, &exitStatus);

  printf("Global Variable: %d\n", global_var);
  return 0;
}
