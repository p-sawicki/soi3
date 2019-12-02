#include "head.h"
int main(int argc, char **argv){
	FILE *logFile, *bufferFile, *fullLogFile;
	char fileName[50] = "consumer";
	int consumeAmount, inMagazine, value, blocked, i;
	int maxInMagazine = atoi(argv[2]);
	int currentTime;
	srand((int)time(0) + atoi(argv[1]));
	sem_init(FILE_MUTEX, 1);
	sem_init(FULL_DELAY, 0);
	sem_init(EMPTY_DELAY, 0);
	strcat(fileName, argv[1]);
	strcat(fileName, ".txt");
	printf("Starting consumer #%s\n", argv[1]);
	logFile = fopen(fileName, "w"); /* clear file */
	fclose(logFile);
	for(i = 0; i < atoi(argv[5]); ++i){
		consumeAmount = uniformDistribution(atoi(argv[3]), atoi(argv[4]));
		sem_down(FILE_MUTEX);
		bufferFile = fopen(BUFFER_FILE, "r");
		inMagazine = getInMagazine(bufferFile);
		while(inMagazine < consumeAmount){
			++i;
			fullLogFile = fopen("fullLog.txt", "a");
			currentTime = (int)time(0);
			fprintf(fullLogFile, "[%d] Consumer #%s can't take %d items.\n", currentTime, argv[1], consumeAmount);
			fclose(fullLogFile); 
			fclose(bufferFile);
			sem_up(FILE_MUTEX);
			printf("Consumer #%s can't take %d items.\n", argv[1], consumeAmount);
			logFile = fopen(fileName, "a");
			fprintf(logFile, "[%d] Can't take %d items.\n", currentTime, consumeAmount);
			fclose(logFile);
			sem_down(EMPTY_DELAY);
			bufferFile = fopen(BUFFER_FILE, "r");
			inMagazine = getInMagazine(bufferFile);
		}
		fullLogFile = fopen("fullLog.txt", "a");
		currentTime = (int)time(0);
		fprintf(fullLogFile, "[%d] Consumer #%s takes %d items.\n", currentTime, argv[1], consumeAmount);
		fprintf(fullLogFile, "[%d] In magazine after: %d.\n\n", currentTime, inMagazine - consumeAmount);
		fclose(fullLogFile);
		printf("Consumer #%s takes %d items.\n", argv[1], consumeAmount);
		logFile = fopen(fileName, "a");
		fprintf(logFile, "[%d] Taking %d items.\n", currentTime, consumeAmount);
		fprintf(logFile, "[%d] In magazine after: %d.\n\n", currentTime, inMagazine - consumeAmount);
		fclose(logFile);
		setInMagazine(bufferFile, inMagazine - consumeAmount);
		sem_status(FULL_DELAY, &value, &blocked);
		fclose(bufferFile);
		sleep(2);
		if(blocked)
			sem_up(FULL_DELAY);
		else
			sem_up(FILE_MUTEX);
	}
	printf("Ending consumer #%s\n", argv[1]);
	sem_init(EMPTY_DELAY, -1);
	sem_init(FULL_DELAY, -1);
	sem_init(FILE_MUTEX, -1);
}
