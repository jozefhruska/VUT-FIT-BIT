#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/* ARGUMENTS:
 * 0 - (R) Rides
 * 1 - (C) Capacity
 * 2 - (ART) Time to generate new Rider
 * 3 - (ABT) Time of simulated ride
 */
int ARGUMENTS[4];

typedef struct {
	sem_t *sem_shared_data;
	sem_t *sem_stop_queue;
	sem_t *sem_bus_queue;
	sem_t *sem_bus_boarded;
	sem_t *sem_finish;
} struct_semaphores;

typedef struct {
	int logCount;
	int pToBoard;
	int busQueue;
	int pTransported;
	int pCount;
	int rWorking;
	FILE *logFile;
} struct_shared_data;

extern struct_semaphores *SEMAPHORES;
extern struct_shared_data *SHARED_DATA;
extern int sharedMemory;

/**
 * @brief  Check if arguments are within bounds and convert them to needed data types.
 * @param  argc: Count of arguments
 * @param  argv[]: Array of arguments from CLI
 */
void parseArgs();

/**
 * @brief  Close all created semaphores.
 */
void closeSemaphores();

/**
 * @brief  CLean all shared data out of memory.
 */
void cleanSharedData();

/**
 * @brief  Wrapper function to initialize arguments, shared data and semaphores.
 * @param  argc: Number of arguments
 * @param  argv[]: Array of arguments
 */
void initData(int argc, char* argv[]);
