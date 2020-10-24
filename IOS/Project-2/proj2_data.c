#include "proj2_data.h"

struct_semaphores *SEMAPHORES = NULL;
struct_shared_data *SHARED_DATA = NULL;
int sharedMemory = 0;

void parseArgs(int argc, char* argv[]) {
	if (argc == 5) {
		// Convert and store values to the ARGUMENTS array
		ARGUMENTS[0] = atoi(argv[1]);
		ARGUMENTS[1] = atoi(argv[2]);
		ARGUMENTS[2] = atoi(argv[3]);
		ARGUMENTS[3] = atoi(argv[4]);

		// Check if arguments are within bounds
		if (ARGUMENTS[0] <= 0 || ARGUMENTS[1] <= 0 || ARGUMENTS[2] < 0 || ARGUMENTS[2] > 1000 || ARGUMENTS[3] < 0 || ARGUMENTS[3] > 1000) {
			fprintf(stderr, "Error: Badly formatted arguments\n");
			exit(EXIT_FAILURE);
		}
	} else {
		fprintf(stderr, "Error: Badly formatted arguments\n");
		exit(EXIT_FAILURE);
	}
}

void initSharedData() {
	if ((sharedMemory = shmget(IPC_PRIVATE, sizeof(struct_shared_data), IPC_CREAT | 0666)) != -1) {
        SHARED_DATA = (struct_shared_data *) shmat(sharedMemory, NULL, 0);
		if (SHARED_DATA != NULL) {
			SHARED_DATA->logCount = 1;
			SHARED_DATA->pToBoard = 0;
			SHARED_DATA->busQueue = 0;
			SHARED_DATA->pTransported = 0;
			SHARED_DATA->pCount = 1;
			SHARED_DATA->logFile = NULL;
			SHARED_DATA->rWorking = ARGUMENTS[0];
			return;
		}
    }

	fprintf(stderr, "Error: Something went wrong while attempting to initialize shared data\n");
	exit(EXIT_FAILURE);
}

void initSemaphores() {
	int err = 0;
	SEMAPHORES = malloc(sizeof(struct_semaphores));
	if (SEMAPHORES == NULL) {
		fprintf(stderr, "Error: Could not allocate memmory for SEMAPHORES\n");
		exit(EXIT_FAILURE);
	}

	if ((SEMAPHORES->sem_shared_data = sem_open("/xhrusk25-proj2-sd", O_CREAT | O_EXCL, DEFFILEMODE, 1)) == SEM_FAILED) err++;
	if ((SEMAPHORES->sem_stop_queue = sem_open("/xhrusk25-proj2-sq", O_CREAT | O_EXCL, DEFFILEMODE, 0)) == SEM_FAILED) err++;
	if ((SEMAPHORES->sem_bus_queue = sem_open("/xhrusk25-proj2-bq", O_CREAT | O_EXCL, DEFFILEMODE, 0)) == SEM_FAILED) err++;
	if ((SEMAPHORES->sem_bus_boarded = sem_open("/xhrusk25-proj2-bd", O_CREAT | O_EXCL, DEFFILEMODE, 0)) == SEM_FAILED) err++;
	if ((SEMAPHORES->sem_finish = sem_open("/xhrusk25-proj2-fh", O_CREAT | O_EXCL, DEFFILEMODE, 0)) == SEM_FAILED) err++;

	if (err != 0) {
		fprintf(stderr, "Error: Something went wrong while attempting to initialize semaphores - %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void closeSemaphores() {
	sem_close(SEMAPHORES->sem_shared_data);
	sem_unlink("/xhrusk25-proj2-sd");

	sem_close(SEMAPHORES->sem_stop_queue);
	sem_unlink("/xhrusk25-proj2-sq");

	sem_close(SEMAPHORES->sem_bus_queue);
	sem_unlink("/xhrusk25-proj2-bq");

	sem_close(SEMAPHORES->sem_bus_boarded);
	sem_unlink("/xhrusk25-proj2-bd");

	sem_close(SEMAPHORES->sem_finish);
	sem_unlink("/xhrusk25-proj2-fh");
}

void cleanSharedData() {
	shmctl(sharedMemory, IPC_RMID, NULL);
}

void initData(int argc, char* argv[]) {

	// Parse and validate program arguments
	parseArgs(argc, argv);

	// Initialize SHARED_DATA structure
	initSharedData();

	// Initialize SEMAPHORES structure
	initSemaphores();
}
