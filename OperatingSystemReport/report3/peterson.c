#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/syscall.h>
#include"peterson.h"

void* runner(void* param);
int sum=0;
//pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
		  //pthread_mutex_init(&mutex, NULL);

		  pthread_t tid[2];
		  pthread_attr_t attr;

		  pthread_attr_init(&attr);

		  pthread_create(&tid[0], &attr, runner, NULL);
		  pthread_create(&tid[1], &attr, runner, NULL);

		  pthread_join(tid[0], NULL);
		  pthread_join(tid[1], NULL);

		  return 0;
}
void* runner(void* param)
{
		  int i;
		  size_t tid=syscall(SYS_gettid);
	  	  //first thread : turn 0
		  //second thread : turn 1  
		  size_t turn=(tid==(getpid()+1)) ? 0 : 1;

		  peterson_lock(turn);
		 // pthread_mutex_lock(&mutex);

		  for(i=0; i<=10000; i++)
					 sum+=i;

		  printf("%d\n", sum);
		  sum=0;
		 // pthread_mutex_unlock(&mutex);
		  peterson_unlock(turn);
}


