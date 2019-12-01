#include <minix/semaphore.h>
int main(int argc, char **argv){
	message m;
	_syscall(MM, SEM_UNINIT, &m);
}
