
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
    sleep_ms(LOG_INTERVAL_MS);
    
    if (RUNTIME_SECONDS < LOG_INTERVAL_MS + DELAY_THRESH)
    {
      printf("logging achieved at %d seconds.\n", RUNTIME_SECONDS);
    }
    else 
    {
      printf("Deadline missed");
    }
    return NULL;
}

int main() {

	/* necessary commands for thread creation */
    pthread_t threadID;
    void* exitStatus;
    int value = 42;
    
    for(int i = 0; i <= 20; i ++) 
    {
      pthread_create(&threadID, NULL, logger_thread, &value);
      pthread_join(threadID, &exitStatus);
    }

    return 0;
}
