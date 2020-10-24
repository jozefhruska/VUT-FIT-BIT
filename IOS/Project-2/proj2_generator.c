#include "proj2_generator.h"
#include "proj2_data.h"

void initRiderProcess() {
	int RID = 0;
	sem_wait(SEMAPHORES->sem_shared_data);
		RID = SHARED_DATA->pCount++;

		// Log start message
		fprintf(SHARED_DATA->logFile, "%d\t: RID %d\t: start\n", SHARED_DATA->logCount++, RID); fflush(SHARED_DATA->logFile);
	sem_post(SEMAPHORES->sem_shared_data);

	sem_wait(SEMAPHORES->sem_stop_queue);

		sem_wait(SEMAPHORES->sem_shared_data);
			// Log enter message
			fprintf(SHARED_DATA->logFile, "%d\t: RID %d\t: enter: %d\n", SHARED_DATA->logCount++, RID, ++(SHARED_DATA->busQueue)); fflush(SHARED_DATA->logFile);
		sem_post(SEMAPHORES->sem_shared_data);

	sem_post(SEMAPHORES->sem_stop_queue);

	sem_wait(SEMAPHORES->sem_bus_queue);
		sem_wait(SEMAPHORES->sem_shared_data);
			SHARED_DATA->busQueue--;

			// Log boarding message
			fprintf(SHARED_DATA->logFile, "%d\t: RID %d\t: boarding\n", SHARED_DATA->logCount++, RID); fflush(SHARED_DATA->logFile);
		sem_post(SEMAPHORES->sem_shared_data);
	sem_post(SEMAPHORES->sem_bus_boarded);

	sem_wait(SEMAPHORES->sem_finish);
		sem_wait(SEMAPHORES->sem_shared_data);
			// Log finish message
			fprintf(SHARED_DATA->logFile, "%d\t: RID %d\t: finish\n", SHARED_DATA->logCount++, RID); fflush(SHARED_DATA->logFile);
		sem_post(SEMAPHORES->sem_shared_data);
	sem_post(SEMAPHORES->sem_finish);

	exit(EXIT_SUCCESS);
}

/**
 * @brief  Initialize Rider Generator process. Generates new Rider processes.
 * @return PID of Generator process
 */
pid_t initRiderGenerator() {
	// Create new generator process
	pid_t genPID = fork();
	if (genPID < 0) {
		fprintf(stderr, "Error: Something went wrong while attempting to create new RiderGenerator process\n");
		exit(EXIT_FAILURE);
	} else if (genPID == 0) { // Generator process
		for (int RID = 1; RID <= ARGUMENTS[0]; RID++) {
			if (ARGUMENTS[2] != 0) pSleep(ARGUMENTS[2]);

			pid_t child_pod = fork();
			if (child_pod < 0) {
				fprintf(stderr, "Error: Something went wrong while attempting to create new Rider process\n");
				exit(EXIT_FAILURE);
			} else if (child_pod == 0) { // Rider process
				initRiderProcess();
			}
		}

		// passive wait for all child processes
		while (wait(NULL)) {
			if (errno == ECHILD) {
				// no child processes
				break;
			}
		}

		exit(EXIT_SUCCESS);
	} else return genPID;
}

void pSleep(int value) {
	if (value > 0) {
		unsigned long time = (rand() % value);
		if (time > 0) usleep((useconds_t) (time * 1000));
	}
}
