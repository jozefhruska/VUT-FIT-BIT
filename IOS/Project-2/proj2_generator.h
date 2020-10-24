#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

/**
 * @brief  Initialize new Rider process
 */
pid_t initRiderGenerator();

/**
 * @brief  Sleep process for time from 0 to 'value'.
 * @param  value: Maximum sleep duration
 */
void pSleep();
