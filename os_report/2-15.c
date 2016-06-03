#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#define BUF_SIZE 100
#define NAME_SIZE 20

void error_handling(char* message);

int main(int argc, char* argv[])
{
	char buf[BUF_SIZE]={0};
	char buf1[]="input source file name : ";
	char buf2[]="input destination file name : ";
	char source[NAME_SIZE]={0};
	char destination[NAME_SIZE]={0};
	int source_fd, dest_fd, read_cnt;

	//0 : standard input
	//1 : standard output
	write(1,buf1,sizeof(buf1));
	read(0,source,sizeof(source));
	write(1,buf2,sizeof(buf2));
	read(0,destination,sizeof(destination));

	source[strlen(source)-1]='\0';
	destination[strlen(destination)-1]='\0';

	//if file doesn't exist, abort
	source_fd=open(source, O_RDONLY);
	if(source_fd==-1)
			  error_handling("source file open() error!");
	
	//if file exists, abort
	//chmod 644 
	dest_fd=open(destination, O_CREAT | O_WRONLY | O_EXCL, 0644);
	if(dest_fd==-1)
			  error_handling("destination file open() error!");

	//write behavior
	while((read_cnt=read(source_fd, buf, sizeof(buf)))>0)
			  write(dest_fd, buf, read_cnt);

	write(1,"write completion\n", 17);
	close(source_fd);
	close(dest_fd);

	return 0;
}
void error_handling(char *message)
{
		 write(2, message, strlen(message));
		 write(2,"\n",1);
		 abort();
}
			
