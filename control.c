#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define KEY 24601

union semun {
  int val ;
  struct semid_ds *buf ;
  unsigned short *array ;
  struct seminfo *__buf ;
} ;

void create() {
  printf("We are going to create the story now!\n") ;
  int shmid = shmget(KEY, sizeof(int), 0644 | IPC_CREAT) ;
  if (shmid == -1) printf("Error with creating shared memory! %d: %s\n", errno , strerror(errno)) ;

  int fd = open("story.txt", O_CREAT | O_TRUNC, 0644) ;
  if (fd == -1) printf("Error %d: %s\n", errno , strerror(errno)) ;

  int semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644) ;
  if (semd == -1) printf("Error %d: %s\n", errno , strerror(errno)) ;
  
  union semun us ;
  us.val = 1 ;
  int r = semctl(semd, 0, SETVAL, us) ;
  if (r == -1) printf("Error %d: %s\n", errno, strerror(errno)) ;
}

int main() {
  return 0 ;
}
