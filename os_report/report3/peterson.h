#ifndef __PETERSON_H__
#define __PETERSON_H__
#define TRUE 1
#define FALSE 0
int turn;
int flag[2];

void peterson_init()
{
		  flag[0]=flag[1]=FALSE;
}
void peterson_lock(int tid)
{
		  int p;
		  flag[tid]=TRUE;

		  if(tid > 0)
					 p=tid-1;
		  else
					 p=tid+1;
		  turn=p;

		  while(flag[p]&& turn==p);
}
void peterson_unlock(int tid)
{
		  flag[tid]=FALSE;
}

#endif
