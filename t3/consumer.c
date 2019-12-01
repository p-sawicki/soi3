#include "head.h"
FILE *logFile;
FILE *bufferFile;
int main(int argc, char **argv){
	char fileName[50] = "consumer";
	int consumeAmount, magazineSpace, value, blocked, i;
	int maxInMagazine = atoi(argv[2]);
	srand(time(0));
	sem_init(FILE_MUTEX, 1);
	sem_init(FULL_DELAY, 0);
	sem_init(EMPTY_DELAY, 0);
	strcat(fileName, argv[1]);
	strcat(fileName, ".txt");
	logFile = fopen(fileName, "w");
	printf("Starting consumer #%s\n", argv[1]);
	for(i = 0; i < atoi(argv[5]); ++i){
		consumeAmount = uniformDistribution(atoi(argv[3]), atoi(argv[4]));
		sem_down(FILE_MUTEX);
		bufferFile = fopen(BUFFER_FILE, "r");
		magazineSpace = getMagazineSpace(bufferFile);
		while(maxInMagazine - magazineSpace < consumeAmount){
			fclose(bufferFile);
			sem_up(FILE_MUTEX);
			printf("Consumer #%s can't take %d items.\n", argv[1], consumeAmount);
			fprintf(logFile, "[%d] Can't take %d items.\n", (int)time(0), consumeAmount);
			sleep(1);
			sem_down(EMPTY_DELAY);
			fopen(BUFFER_FILE, "r");
			magazineSpace = getMagazineSpace(bufferFile);
		}
		printf("Consumer #%s takes %d items.\n", argv[1], consumeAmount);
		fprintf(logFile, "[%d] Taking %d items.\n", (int)time(0), consumeAmount);
		fprintf(logFile, "[%d] Space in magazine after: %d.\n\n", (int)time(0), magazineSpace + consumeAmount);
		setMagazineSpace(bufferFile, magazineSpace + consumeAmount);
		sleep(1);
		sem_status(FULL_DELAY, &value, &blocked);
		fclose(bufferFile);
		if(blocked)
			sem_up(FULL_DELAY);
		else
			sem_up(FILE_MUTEX);
	}
	printf("Ending consumer #%s\n", argv[1]);
	fclose(bufferFile);
	fclose(logFile);
	sem_init(EMPTY_DELAY, -1);
	sem_init(FULL_DELAY, -1);
	sem_init(FILE_MUTEX, -1);
}
