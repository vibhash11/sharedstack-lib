#include"shstacks.h"

#ifndef MAX_STACKS
#define MAX_STACKS 10
#endif

int shmuniv;
stackdesc *shared_stacks[MAX_STACKS];

int shstackget(key_t key,int data_size, int stack_size){
	int i;
	key_t univkey = ftok("/mnt/c/Users/Vibhash Chandra/Desktop/Study/Git/Operating System Lab Git/Shared Stack/univKey", 1); //universal key for each process which includes this library	
	/*
		Allocating shared memory segment to each element in the array of stackdesc pointers
		Total size required for the stack descriptors:
			size = sizeof(int)*1 <- Keeps track of whether the array stack descriptor pointers is initialized or not
			       + sizeof(stackdesc)*MAX_STACKS <- Memory required for the array of stackdesc pointers
	*/
	int size = sizeof(int)+sizeof(stackdesc)*MAX_STACKS; 
	shmuniv = shmget(univkey,(size_t)size,IPC_CREAT|0777);
	void *univ = shmat(shmuniv,NULL,0);
	int *in = (int *)univ;
	univ += sizeof(int);
	stackdesc *desc = (stackdesc *)univ;
	for(i=0;i<MAX_STACKS;i++)
	{
		shared_stacks[i] = desc+i;
		if(*in == 0){
			shared_stacks[i]->stackKey = -1; // initially the stack does not have any key associated with it
			shared_stacks[i]->curr_elements = 0; //setting current number of elements = 0
			shared_stacks[i]->free = 1; // this descriptor is now available for use
			shared_stacks[i]->stack_top = NULL; // setting stack top pointer to NULL
		}
	}
	*in = 1; // stating that the stackdesc *shared_stacks[MAX_STACKS] is initialized
	
	/* checking if there is any stack having same key as passed in the argument */		
	for(i=0;i<MAX_STACKS;i++)
		if(shared_stacks[i]->stackKey==key){
			printf("Stack with given key found!\n");				
			break;
		}
	/* if the key is not found in the stack descriptor */
	if(i==MAX_STACKS){
		/* checking if any stack descriptor is free */
		for(i=0;i<MAX_STACKS;i++){
			if(shared_stacks[i]->free)
				break;
		}
		/* couldn't find any stack which is free to use */
		if(i==MAX_STACKS){
			return -1;
		}
		/* creating stack and initializing the ith stack descriptor */
		int shmid = shmget(key,data_size*stack_size,IPC_CREAT|0777);
		shared_stacks[i]->stackKey = key;
		shared_stacks[i]->data_size = data_size;		
		shared_stacks[i]->stack_size = stack_size;			
		shared_stacks[i]->shmid = shmid;
		shared_stacks[i]->free = 0; // this descriptor is now unavailable for use if stackKey!=key sent by some other process		
	}
	return i;
}

int shstackpush(int id, void *data){
	if(shared_stacks[id]->curr_elements==shared_stacks[id]->stack_size){ // checks if stack is full.
		printf("Overflow!\n");
		return -1;
	}
	void *stack = shmat(shared_stacks[id]->shmid,NULL,0);	
	shared_stacks[id]->stack_top = stack + shared_stacks[id]->data_size*shared_stacks[id]->curr_elements;
	memcpy(shared_stacks[id]->stack_top,data,shared_stacks[id]->data_size);	// copy data from void * to stack top
	shared_stacks[id]->curr_elements+=1;
	shmdt(stack);
	return 0;
}

int shstackpop(int id, void *popped){		
	if(shared_stacks[id]->curr_elements==0) {
		printf("Underflow!\n");
		return -1;
	}	
	void *stack = shmat(shared_stacks[id]->shmid,NULL,0);				
	shared_stacks[id]->curr_elements-=1;
	shared_stacks[id]->stack_top = stack + shared_stacks[id]->data_size*shared_stacks[id]->curr_elements;	
	memcpy(popped,shared_stacks[id]->stack_top,shared_stacks[id]->data_size); // copy popped value to void * from stack top
	shmdt(stack);
	return 0;
}

void shstackrm(int id){
	shared_stacks[id]->stackKey = -1;
	shared_stacks[id]->curr_elements = 0;
	shared_stacks[id]->free = 1;
	shmctl(shared_stacks[id]->shmid,IPC_RMID,NULL);
}
