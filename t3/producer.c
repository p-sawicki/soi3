#include "head.h"
FILE *logFile;
FILE *bufferFile;
int main(int argc, char **argv){
	char fileName[50] = "producer";
	int produceAmount, magazineSpace, value, blocked, i;
	int maxInMagazine = atoi(argv[2]);
	srand(time(0));
	sem_init(FILE_MUTEX, 1);
	sem_init(FULL_DELAY, 0);
	sem_init(EMPTY_DELAY, 0);
	strcat(fileName, argv[1]);
	strcat(fileName, ".txt");
	logFile = fopen(fileName, "w");
	printf("Starting producer #%s.\n", argv[1]);
	for(i = 0; i < atoi(argv[5]); ++i){
		produceAmount = uniformDistribution(atoi(argv[3]), atoi(argv[4]));
		sem_down(FILE_MUTEX);
		bufferFile = fopen(BUFFER_FILE, "r");
		magazineSpace = getMagazineSpace(bufferFile);
		while(magazineSpace < produceAmount){
			fclose(bufferFile);
			sem_up(FILE_MUTEX);
			printf("Producer #%s can't insert %d items due to lack of space.\n", argv[1], produceAmount);
			fprintf(logFile, "[%d] Can't insert %d items due to lack of space.\n", (int)time(0), produceAmount);
			sleep(1);
			sem_down(FULL_DELAY);
			fopen(BUFFER_FILE, "r");
			magazineSpace = getMagazineSpace(bufferFile);
		}
		printf("Producer #%s inserts %d items.\n", argv[1], produceAmount);
		fprintf(logFile, "[%d] Inserted %d items.\n", (int)time(0), produceAmount);
		setMagazineSpace(bufferFile, magazineSpace - produceAmount);
		fprintf(logFile, "[%d] Space in magazine after insertion: %d.\n\n", (int)time(0), magazineSpace - produceAmount);
		sleep(1);
		sem_status(EMPTY_DELAY, &value, &blocked);
		fclose(bufferFile);
		if(blocked)
			sem_up(EMPTY_DELAY);
		else
			sem_up(FILE_MUTEX);
	}
	printf("Ending producer #%s.\n", argv[1]);
	fclose(bufferFile);
	fclose(logFile);
	sem_init(EMPTY_DELAY, -1);
	sem_init(FULL_DELAY, -1);
	sem_init(FILE_MUTEX, -1);
}
