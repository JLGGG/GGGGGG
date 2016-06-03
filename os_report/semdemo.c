/*  semdemo.c  */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <stdio.h>
extern int errno;
#define SEMPERM 0600//permission
#define TRUE 1
#define FALSE 0
int main()
{	
	key_t semkey=0x500;//semaphore key
	if (fork()==0) handlesem(semkey);	
	if (fork()==0) handlesem(semkey);
	if (fork()==0) handlesem(semkey);
	sleep(10);
	printf("parent %d exit\n", getpid());
	return 0;
}
/* initsem – semaphore initialization */
int initsem(key_t semkey)
{	
	int status=0, semid;
	/*permission 0600 : read-only users
	 IPC_CREAT creates a semaphore
	 IPC_EXCL은 IPC_REAT와 함께사용. 세마포어가 있으면 에러 반환 */
	if ((semid=semget(semkey, 1, SEMPERM|IPC_CREAT|IPC_EXCL)) == -1)	{
		/*이미 존재할 경우 semid만 얻는다.*/
			  if (errno == EEXIST)
				semid = semget(semkey, 1, 0);	

	} else status = semctl(semid, 0, SETVAL, 1);//세마포어를 1로 초기화
		if (semid == -1||status == -1)  {
				perror ("initsem failed");
				return(-1);	
	} else return semid;
}

int handlesem(key_t skey)
{	
	int semid, pid=getpid();
   if ((semid = initsem(skey)) < 0) exit(1);
				
	printf("\n process %d before critical section\n", pid);
	p(semid);//wait operation
	//-------------------------------------------------
	//critical section!!
	printf("process %d in critical section\n", pid);
	sleep(2);
	//-------------------------------------------------
	v(semid);//signal operation
	printf("process %d existing\n", pid);
	exit(0);
}
int p(int semid)
{
	/*wait operation*/
	struct sembuf p_buf;
	p_buf.sem_op = -1;//semaphore operation(wait role)
	p_buf.sem_num = 0;//semaphore index in array
	p_buf.sem_flg = SEM_UNDO;//operation flags
	if (semop(semid, &p_buf, 1) == -1)  {
			perror("p(semid) failed");
			exit(1);
	}  else return(0);
}
int v(int semid)
{
	/*signal operation*/
	struct sembuf v_buf;
	v_buf.sem_num = 0;//semaphore index in array
	v_buf.sem_op = 1;//semaphore operation(signal role)
	v_buf.sem_flg = SEM_UNDO;//operation flags
	if (semop(semid, &v_buf, 1) == -1)  {
			perror("v(semid) failed");
			exit(1);
	}	else return(0);
}

