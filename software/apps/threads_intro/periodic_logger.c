
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define LOG_INTERVAL_MS 1000
#define RUNTIME_SECONDS 100
#define DELAY_THRESH 5

void sleep_ms(int ms) {
    struct timespec ts = {ms / 1000, (ms % 1000) * 1000000};
    nanosleep(&ts, NULL);
}

void* logger_thread(void* arg) {
    
    /* Implement a code that runs every LOG_INTERVAL_MS
    Print a message if it misses an interval by DELAY_THRESH */
    struct timespec start, end;
    double elapsed_ms;

    clock_gettime(CLOCK_MONOTONIC, &start);
    
    sleep_ms(LOG_INTERVAL_MS);
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    elapsed_ms = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_nsec - start.tv_nsec) / 1000000.0);
    
    if (elapsed_ms < LOG_INTERVAL_MS + DELAY_THRESH)
    {
      printf("logging achieved at %f seconds.\n", elapsed_ms / 1000.0);
    }
    else 
    {
      printf("Deadline missed\n");
    }
    return NULL;
}

void * stress_thread(void* arg) 
{
  volatile double funnyAnswer = 1.0;
  
  while (1) 
  {
    funnyAnswer = funnyAnswer * 67000.00000067 + 0.0000000000000000067 / 670000.00000000000000067;
  }

  return NULL;
}

int main() {

	/* necessary commands for thread creation */
    pthread_t threadID, stressID;
    void* exitStatus;
    int value = 67;
    
    pthread_create(&stressID, NULL, stress_thread, NULL);

    for(int i = 0; i <= 20; i ++) 
    {
      pthread_create(&threadID, NULL, logger_thread, &value);
      pthread_join(threadID, &exitStatus);
    }

    return 0;
}
