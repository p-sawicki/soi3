#include "head.h"
/* resets all semaphores, even ones still in use 
so they can be used again after a deadlock, 
kill all processes using semaphores before running this*/
int main(int argc, char **argv){
	message m;
	_syscall(MM, SEM_UNINIT, &m); 
}
