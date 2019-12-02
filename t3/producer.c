#include "head.h"
int main(int argc, char **argv){
	FILE *logFile, *bufferFile, *fullLogFile;
	char fileName[50] = "producer";
	int produceAmount, inMagazine, value, blocked, i;
	int maxInMagazine = atoi(argv[2]);
	int currentTime;
	srand((int)time(0) + 420 * atoi(argv[1]));
	sem_init(FILE_MUTEX, 1);
	sem_init(FULL_DELAY, 0);
	sem_init(EMPTY_DELAY, 0);
	strcat(fileName, argv[1]);
	strcat(fileName, ".txt");
	printf("Starting producer #%s.\n", argv[1]);
	logFile = fopen(fileName, "w"); /* clear file */
	fclose(logFile);
	for(i = 0; i < atoi(argv[5]); ++i){
		produceAmount = uniformDistribution(atoi(argv[3]), atoi(argv[4]));
		sem_down(FILE_MUTEX);
		bufferFile = fopen(BUFFER_FILE, "r");
		inMagazine = getInMagazine(bufferFile);
		while(maxInMagazine - inMagazine < produceAmount){
			++i;
			fclose(bufferFile);
			fullLogFile = fopen("fullLog.txt", "a");
			currentTime = (int)time(0);
			fprintf(fullLogFile, "[%d] Producer #%s can't insert %d items due to lack of space.\n", currentTime, argv[1], produceAmount);
			fclose(fullLogFile);
			sem_up(FILE_MUTEX);
			printf("Producer #%s can't insert %d items due to lack of space.\n", argv[1], produceAmount);
			logFile = fopen(fileName, "a");
			fprintf(logFile, "[%d] Can't insert %d items due to lack of space.\n", currentTime, produceAmount);
			fclose(logFile);
			sem_down(FULL_DELAY);
			bufferFile = fopen(BUFFER_FILE, "r");
			inMagazine = getInMagazine(bufferFile);
		}
		currentTime = (int)time(0);
		printf("Producer #%s inserts %d items.\n", argv[1], produceAmount);
		logFile = fopen(fileName, "a");
		fprintf(logFile, "[%d] Inserted %d items.\n", currentTime, produceAmount);
		setInMagazine(bufferFile, inMagazine + produceAmount);
		fprintf(logFile, "[%d] In magazine after insertion: %d.\n\n", currentTime, inMagazine + produceAmount);
		fclose(logFile);
		fullLogFile = fopen("fullLog.txt", "a");
		fprintf(fullLogFile, "[%d] Producer #%s inserted %d items.\n", currentTime, argv[1], produceAmount);
		fprintf(fullLogFile, "[%d] In magazine after insertion: %d.\n\n", currentTime, inMagazine + produceAmount);
		fclose(fullLogFile);
		fclose(bufferFile);
		sem_status(EMPTY_DELAY, &value, &blocked);
		sleep(2);
		if(blocked)
			sem_up(EMPTY_DELAY);
		else
			sem_up(FILE_MUTEX);
	}
	printf("Ending producer #%s.\n", argv[1]);
	fclose(logFile);
	sem_init(EMPTY_DELAY, -1);
	sem_init(FULL_DELAY, -1);
	sem_init(FILE_MUTEX, -1);
}
