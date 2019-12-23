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

// command line argument: -c
int create() {
  printf("We are going to create the story now!\n") ;
  int shmid = shmget(KEY, sizeof(int), 0644 | IPC_CREAT) ;
  if (shmid == -1) {
    printf("Error with creating shared memory!: %s\n", strerror(errno)) ;
    return -1 ;
  }

  int fd = open("story.txt", O_CREAT | O_TRUNC, 0644) ;
  if (fd == -1) {
    printf("Error: %s\n", strerror(errno)) ;
    return -1 ;
  }

  int semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644) ;
  if (semd == -1) {
    printf("Error: %s\n", strerror(errno)) ;
    return -1 ;
  }

  union semun us ;
  us.val = 1 ;
  int r = semctl(semd, 0, SETVAL, us) ;
  if (r == -1) {
    printf("Error: %s\n", strerror(errno)) ;
    return -1 ;
  }
  return 0 ;
}

// command line argument: -r
int remove() {
  int semd = semget(KEY, 1, 0) ;
  struct sembuf sb = calloc(sizeof(struct sembuf), 1) ;
  sb.sem_num = 0 ;
  sb.sem_op = -1 ;
  sb.sem_flg = SEM_UNDO ;
  int sems = semop(semd, sb, 1) ;
  free(sb) ;

  // show the story
  int fd = open("story.txt", O_RDONLY) ;
  char * story = calloc(sizeof(char), 20480) ;
  read(fd, story, 20479) ;
  printf("Story:\n%s\n", story) ;
  free(story) ;

  int shmd = shmget(KEY, sizeof(int), 0) ;
  int sh = shmctl(shmd, IPC_RMID, NULL) ;
  if (sh == -1) {
    printf("Error: %s\n", strerror(errno)) ;
    return -1 ;
  }

  // ultimate removal - remove shared memory, remove semaphore
  int semmm = semctl(semd, 0, IPC_RMID, NULL) ;
  if (semmm == -1) {
    printf("Error: %s\n", strerror(errno)) ;
    return -1 ;
  }
  int r = remove("story.txt") ;
  if (r == -1) {
    printf("Error: %s\n", strerror(errno)) ;
    return -1 ;
  }
  return 0 ;
}


int main() {
  return 0 ;
}
