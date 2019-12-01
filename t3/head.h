#include <stdio.h>
#include <time.h>
#include <minix/semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define FILE_MUTEX 0
#define FULL_DELAY 1
#define EMPTY_DELAY 2
#define BUFFER_FILE "magazine.txt"
int getInMagazine(FILE *f){
	int inMagazine = 0;
	fscanf(f, "%d", &inMagazine);
	return inMagazine;
}
void setInMagazine(FILE *f, int amount){
	fclose(f);
	f = fopen(BUFFER_FILE, "w");
	fprintf(f, "%d", amount);
	printf("Currently in magazine: %d\n", amount);
}
int uniformDistribution(int min, int max){
	return min + rand() % (max - min + 1);
}
int sem_uninit(void){
	message m;
	return _syscall(MM, SEM_UNINIT, &m);
}
