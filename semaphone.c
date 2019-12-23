#include "control.h"

int main() {
  int semd = semget(KEY, 1, 0);
  struct sembuf sb ;
  sb.sem_num = 0 ;
  sb.sem_op = -1 ;
  sb.sem_flg = SEM_UNDO ;
  int sem_opp = semop(semd, sb, 1);

  int shmd = shmget(KEY, sizeof(int), 0644) ;
  if (shmd == -1) {
    printf("Error with shmget: %s\n", strerror(errno)) ;
    return 1 ;
  }

  return 0 ;
}
