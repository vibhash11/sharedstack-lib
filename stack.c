#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include"shstacks.h"

int stackid;

typedef void (*sighandler_t)(int);
void freeSH(int signum) {
	shstackrm(stackid);
}

int main(){
	signal(SIGINT, freeSH);
	key_t mykey = ftok("/mnt/c/Users/Vibhash Chandra/Desktop/Study/Git/Operating System Lab Git/Shared Stack/key", 1);
	stackid = shstackget(mykey, sizeof(float), 5);
	printf("Stack id: %d\n", stackid);
	printf("1.Push\n2.Pop\n3.Exit\n4.Exit and Close stack descriptor with Stack Id: %d\n", stackid);
	int ch;
	float *data;
	float *popped;
	while(1){
		printf("Enter Choice: ");
		scanf("%d",&ch);		
		if(ch==1){
			printf("Push Selected.\nEnter data to push: ");
			scanf("%f",data);
			shstackpush(stackid,(void *)data);
		}
		else if(ch==2){
			if(shstackpop(stackid,(void *)data)==0)
				printf("Popped: %f\n",*(float *)data);
			}
		else if(ch==3) {
			break;
		}
		else if(ch==4){
			shstackrm(stackid);
			break;		
		}
		else printf("Wrong Choice.\n");
	}			
	return 0;
}	
	
