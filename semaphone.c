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

int main() {
  int semd = semget(KEY, 1, 0);
  struct sembuf sb ;
  sb.sem_num = 0 ;
  sb.sem_op = -1 ;
  sb.sem_flg = SEM_UNDO ;
  int sem_opp = semop(semd, &sb, 1);

  int shmd = shmget(KEY, sizeof(int), 0644) ;
  if (shmd == -1) {
    printf("Error with shmget: %s\n", strerror(errno)) ;
    return 1 ;
  }

  int fd = open("story.txt", O_RDWR | O_APPEND) ;
  if (fd == -1) {
    printf("Error with opening story.text: %s\n", strerror(errno)) ;
    return 1 ;
  }
  char * story = calloc(sizeof(char), 20480) ;
  int r = read(fd, story, 20479) ;
  if (r == -1) {
    printf("Error with reading story: %s\n", strerror(errno)) ;
    return 1 ;
  }

  int * data = shmat(shmd, 0, 0) ;
  int l = data[0] ;
  printf("The length of the last line was %d\n", l) ;
  char * line = story + strlen(story) - l ;
  printf("Here is the last line of the story: \n%s\n", line) ;

  printf("Your line:\n") ;
  char * new = calloc(sizeof(char), 2049) ;
  fgets(new, 2048, stdin) ;
  new[2047] = '\n' ;
  int l2 = strlen(new) ;
  data[0] = l2 ;

  int w = write(fd, new, l2) ;
  if (w == -1) {
    printf("Error with writing: %s\n", strerror(errno)) ;
  }

  sb.sem_op = 1 ;
  semop(semd, &sb, 1) ;
  return 0 ;
}
