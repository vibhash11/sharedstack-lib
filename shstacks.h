/***
	Documentation for library shstacks
	
	Name: 
		shstacks - standard functions for working with generic shared stacks
	Description:
		This module provides functions to work with multiple generic shared stacks which allow concurrent access by several 
		processes through function calls specified below. The maximum number of stacks that the module can handle is 10. 
		The stack descriptor structure definition: 
			typedef struct{
				key_t stackKey; // stores key specific to a stack
				int data_size; // size of data that will be inserted 
				int stack_size; // total number of elements in the stack
				void *stack_top; // pointing to the top of the stack
				int curr_elements; // number of elements currently in  thestack
				int free; // whether this descriptor is free to use
				int shmid; // shmid of the shared memory segment i.e. is stack
			}stackdesc;
		An array of stackdesc structure pointers is maintained and shared among process which includes this library.
		This is achieved through the shmget() system call and a public key.
		
		List of functions					Description
		--------------------------------------------------------------------------------------------------------------		
		int shstackget(key_t,int,int)		allocates an stack descriptor associated with the value of argument key.
							returns an identifier for the allocated stack descriptor if successful,
							returns -1 otherwise.

		int shstackpush(int,void *)		pushes value at the address pointed by void * in the stack associated
							with the stack descriptor (int argument).
							returns 0 if operation successful, -1 otherwise.

		int shstackpop(int,void *)		pops value from the stack associated with the stack descriptor (int argument)
						        into the address pointer by void *. 
							returns 0 if successful, -1 otherwise. 

		void shstackrm(int)			resets the stack descriptor(int argument) and marks the shared memory
							segment having shmid in the stack descriptor to be destroyed.
	
	Warnings: Since the stack is generic, appropriate handling of the void pointer should be ensured by the user.
		  If one process marks the shared stack segment to be destroyed using the function call shstackrm, any attempt to
		  access the stack (shstackpush or shstackpop functions) will throw an error(such as Segmentation fault).
	
***/


#define MAX_STACKS 10
#include<stdio.h>
#include<sys/ipc.h> /* shmget(2) */
#include<sys/shm.h> /* shmget(2) shmat(2) */
#include<sys/types.h> /* shmat(2) */
#include<sys/sem.h> /* semget(2) semctl(2) */
#include<string.h>

typedef struct{
	key_t stackKey; /* stores key specific to a stack*/
	int data_size; /* size of data that will be inserted */
	int stack_size; /* total number of elements in the stack */
	void *stack_top; /* pointing to the top of the stack */
	int curr_elements; /* number of elements currently in  thestack */
	int free; /* whether this descriptor is free to use */
	int shmid; /* shmid of the shared memory segment i.e. is stack */
}stackdesc;


/* initializing structure */
extern stackdesc *shared_stacks[MAX_STACKS];

/** 	Function: int shstackget(key_t key,int data_size, int stack_size);
	Parameters: 1.key_t key - A Private or Shared key, 
		    2.int data_size - size of each element in the stack,
		    3.int stack_size - size of the stack or total elements that can be in the stack.
	Returns: An integer value - Stack descripter identifier or -1 if there does not exist any free descripter.
**/ 
	 
int shstackget(key_t key,int data_size, int stack_size);

/**	Function: int shstackpush(int id, void *data);
	Parameters: 1. int id - Stack descripter identifier obtained after shstackget() function call,
		    2. void *data - pointer to the address from which the data will be pushed into the stack.
	Returns: An integer value - 0 if operation successful, -1 otherwise.
**/
	 
int shstackpush(int stackid, void *data);

/**	Function: int shstackpop(int stackid,void *popped);	
	Parameters: 1. int id - Stack descripter identifier obtained after shstackget() function call,
		    2. void *data - pointer to the address to which the data will be popped from the stack.
	Returns: An intger value - 0 if operation successful, -1 otherwise.
**/

int shstackpop(int stackid, void *popped);

/**	Function: int shstackrm(int stackid);
	Parameters: 1. int id - Stack descripter identifier obtained after shstackget() function call.
	Returns: Nothing.
**/

void shstackrm(int stackid);

