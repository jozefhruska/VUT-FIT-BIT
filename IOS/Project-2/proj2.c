#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "proj2_generator.h"
#include "proj2_data.h"

/**
 * @brief  Initialize Bus process.
 * @return PID of Bus process
 */
pid_t initBusProcess() {
	pid_t iBus = fork();
	if (iBus == 0) {
		sem_wait(SEMAPHORES->sem_shared_data);

			// Log start message
			fprintf(SHARED_DATA->logFile, "%d\t: BUS\t: start\n", SHARED_DATA->logCount++); fflush(SHARED_DATA->logFile);

			// Log arrival message
			fprintf(SHARED_DATA->logFile, "%d\t: BUS\t: arrival\n", SHARED_DATA->logCount++); fflush(SHARED_DATA->logFile);

			// Log depart message
			fprintf(SHARED_DATA->logFile, "%d\t: BUS\t: depart\n", SHARED_DATA->logCount++); fflush(SHARED_DATA->logFile);

		sem_post(SEMAPHORES->sem_shared_data);

		while(SHARED_DATA->pTransported < ARGUMENTS[0]) {
			sem_post(SEMAPHORES->sem_stop_queue);
			if (ARGUMENTS[3] != 0) pSleep(ARGUMENTS[3]);
			sem_wait(SEMAPHORES->sem_stop_queue);

			sem_post(SEMAPHORES->sem_finish);

			sem_wait(SEMAPHORES->sem_shared_data);

				// Log end message
				fprintf(SHARED_DATA->logFile, "%d\t: BUS\t: end\n", SHARED_DATA->logCount++); fflush(SHARED_DATA->logFile);

				// Log end message
				fprintf(SHARED_DATA->logFile, "%d\t: BUS\t: arrival\n", SHARED_DATA->logCount++); fflush(SHARED_DATA->logFile);

			sem_post(SEMAPHORES->sem_shared_data);

			sem_wait(SEMAPHORES->sem_finish);

			if (SHARED_DATA->busQueue != 0) {
				sem_wait(SEMAPHORES->sem_shared_data);
					// Log start boarding message
					fprintf(SHARED_DATA->logFile, "%d\t: BUS\t: start boarding: %d\n", SHARED_DATA->logCount++, SHARED_DATA->busQueue); fflush(SHARED_DATA->logFile);

					if (SHARED_DATA->busQueue > ARGUMENTS[1]) SHARED_DATA->pToBoard = ARGUMENTS[1];
					else SHARED_DATA->pToBoard = SHARED_DATA->busQueue;

					SHARED_DATA->pTransported += SHARED_DATA->pToBoard;
				sem_post(SEMAPHORES->sem_shared_data);

				for (int i = 0; i < SHARED_DATA->pToBoard; i++) {
					sem_post(SEMAPHORES->sem_bus_queue);
					sem_wait(SEMAPHORES->sem_bus_boarded);
				}

				sem_wait(SEMAPHORES->sem_shared_data);
					// Log end boarding message
					fprintf(SHARED_DATA->logFile, "%d\t: BUS\t: end boarding: %d\n", SHARED_DATA->logCount++, SHARED_DATA->busQueue); fflush(SHARED_DATA->logFile);

					// Log depart message
					fprintf(SHARED_DATA->logFile, "%d\t: BUS\t: depart\n", SHARED_DATA->logCount++); fflush(SHARED_DATA->logFile);
				sem_post(SEMAPHORES->sem_shared_data);
			} else {
				sem_wait(SEMAPHORES->sem_shared_data);
					// Log depart message
					fprintf(SHARED_DATA->logFile, "%d\t: BUS\t: depart\n", SHARED_DATA->logCount++); fflush(SHARED_DATA->logFile);
				sem_post(SEMAPHORES->sem_shared_data);
			}
		}

		sem_post(SEMAPHORES->sem_finish);
		sem_wait(SEMAPHORES->sem_shared_data);
			// Log end message
			fprintf(SHARED_DATA->logFile, "%d\t: BUS\t: end\n", SHARED_DATA->logCount++); fflush(SHARED_DATA->logFile);

			// Log finish message
			fprintf(SHARED_DATA->logFile, "%d\t: BUS\t: finish\n", SHARED_DATA->logCount++); fflush(SHARED_DATA->logFile);
		sem_post(SEMAPHORES->sem_shared_data);

		exit(EXIT_SUCCESS);
	} else if (iBus < 0) {
		fprintf(stderr, "Error: Something went wrong while attempting to create new Bus process\n");

		// Close and unlink semaphores
		closeSemaphores();

		// Clean shared memory
		cleanSharedData();

		exit(EXIT_FAILURE);
	} else return iBus;
}

int main(int argc, char* argv[]) {

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	// Seed random number generator
	srandom((unsigned) time(NULL));

	// Initialize data (arguments, semaphores, shared data)
	initData(argc, argv);

	// Open log file
	sem_wait(SEMAPHORES->sem_shared_data);
		SHARED_DATA->logFile = fopen("proj2.out", "w");
		if (SHARED_DATA->logFile == NULL) {
			fprintf(stderr, "Error: Unable to access log files\n");

			// Close and unlink semaphores
			closeSemaphores();

			// Clean shared memory
			cleanSharedData();

			exit(EXIT_FAILURE);
		}
	sem_post(SEMAPHORES->sem_shared_data);

	// Initialize Bus process
	pid_t busPID = initBusProcess();

	// Start Rider process generator
	pid_t genPID = initRiderGenerator(argc, argv);

	// Wait for child processes to finish
	waitpid(genPID, NULL, 0);
	waitpid(busPID, NULL, 0);

	// Close and unlink semaphores
	closeSemaphores();

	// Clean shared memory
	cleanSharedData();

	// Close log file
	fclose(SHARED_DATA->logFile);

	return EXIT_SUCCESS;

}
