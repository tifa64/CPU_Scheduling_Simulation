#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "process.h"
//Passing the array, row# and length of the line read from the matrix
//The columns are calculated inside the function, they are passed as pointer since the funcation is void
void buildMat(int mat[][300], int i, int j, char *line, int *columns) {
    //A temprorary string stores the characters from the line till it hit a space
    int iline = 0, jj = 0, col;
    char tempchar[j];
    int tempint = 0;
    int temp;
    //Then it is converted to a float value and the matrix is built
    for(col = 0; col < j; col++) {
        //We know the end of the number
		if(line[col] == ' ' || col == j-1) {
            //To prevent exceptions
			if(tempchar != NULL) {
                //Convert it from string to float
				tempint = atof(tempchar);
				for (temp=0;temp<j;temp++){
                    tempchar[temp]='\0';
				}
				//Adding it to the matrix
				mat[i][jj++] = tempint;
			}
			iline = 0;
		}
		//This allows us to read matrices without knowing rows# or cols# beforehand
		else
    		tempchar[iline++] = line[col];
    	}
    //We we set the value of the columns
    *columns = jj;
}
//Sorting according to arrvial time
void sortArriv(struct Process* p, int n) {
	//Just a simple bubble sort
    int j,i;
    for(i=1;i<n;i++)
    {
        for(j=0;j<n-i;j++)
        {
            if(p[j].ARRIV >p[j+1].ARRIV)
            {
                struct Process temp = p[j];
                p[j] =p[j+1];
                p[j+1] = temp;
            }
        }
    }
}
//Sorting according to arrvial time
void sortPid(struct Process* p, int n) {
	//Just a simple bubble sort
    int j,i;
    for(i=1;i<n;i++)
    {
        for(j=0;j<n-i;j++)
        {
            if(p[j].PID >p[j+1].PID)
            {
                struct Process temp = p[j];
                p[j] =p[j+1];
                p[j+1] = temp;
            }
        }
    }
}
int main()
{
	//Queue data structure    
	struct Queue *q = initialize(10);
	//Process struct
	struct Process p[300];
    FILE *fp;
	char *line = NULL;
	int mat[300][300], cpu[10], io[10];
	//Used for later iterations
    int i = 0, j = 0;
    //Rows and columns for the 2 input matrices
    int rows = 0, columns = 0;
	size_t len = 0;
	ssize_t read;
	fp = fopen("sample.txt", "r");
    //To prevent exceptions
    if (fp == NULL)
        exit(EXIT_FAILURE);
    //Where we actually read the file
    while ((read = getline(&line, &len, fp)) != -1) {
        //This is how we calculate the rows
        rows ++;
        //Calling the function which convert that line into a 2D float array
        buildMat(mat, i++, read, line, &columns);
    }
	//Closing the file and get rid of the string
    fclose(fp);
    if (line)
        free(line);
	//Putting the matrix in the array of Process struct
	for(i = 0; i < rows;  i++) {
		for(j = 0; j < columns; j++) {
			if(j == 0)
				p[i].PID = mat[i][j];
			else if(j == 1)
				p[i].CPU = mat[i][j];
			else if(j == 2)
				p[i].IO = mat[i][j];
			else
				p[i].ARRIV = mat[i][j];
		}
		processInit(&p[i]);
	}
	//Sorting according to arrival time
	sortArriv(p, rows);
	for(i = 0; i < rows;  i++) {
		printf("%d %d %d %d %s\n", p[i].PID, p[i].CPU, p[i].IO, p[i].ARRIV, p[i].state);
		//Putting the CPU burst time in order to retrieve it back to excute the process again
		cpu[i] = p[i].CPU;
		//To handle the special case of having I/O time of zero
		io[i] = p[i].IO;
	}
	/**
	lineNum: To print the line's number
	occupied: A flag to determine if the CPU has a process running or not
	sz: We print the status of the processes in the Queue ONLY when we check for the last element in the queue, so
	we dequeue untill sz == queueSZ(q)
	innerSZ: In order to iterate throught the queue
	n: To know when to exit the loop
	busy: A counter to increment to help us calculate CPU Utilization 
	**/
	int lineNum = 0, occupied = 0, sz = 0, innerSZ = 0, n = rows, busy = 0;
	//Just for iteration
	i = 0, j = 0;
	while(n) {
		/**
		First, we iterate through the process in order to determine which is going to the queue
		to be assigned "ready", which is going to be "blocked" according to its number of CPU bursts
		and which is going to be "running"		
		**/		
		for(i = 0; i < rows ; i++) {
			//This means that the process has been ran, blocked and ran again, so no need for it anymore
			if(strcmp(p[i].state, "done") == 0)
				continue;
			//This means it is new and fresh
			if(p[i].ARRIV == lineNum && !strcmp(p[i].state, "No") ) {
				//If CPU is busy running another process then put it in the waiting queue and set its state to "ready"
				if(occupied) {
					strcpy(p[i].state, "ready");
					enqueue(q, p[i]);
                }
				//Otherwise run it and flag that the CPU is occupied
				else {
					strcpy(p[i].state, "running");
					occupied = 1;
				}
			}
		}
		//Print the line's number
		printf("%d", lineNum++);
		//In this loop, we determine if any new changes has occured to the processes and print them
		for(i = 0; i < rows;  i++) {
			//If the process hasn't been in the CPU nor blocked nor in the queue yet, or it has been through all that; ignore it
			if(!strcmp(p[i].state, "No") || !strcmp(p[i].state, "done")) {
				continue;
			}
			//First case is that if the process is inside the CPU
			if(!strcmp(p[i].state, "running")) {
				//It finished its CPU
				if(!p[i].CPU) {
					//Has it also finished all of its IO cycles
					// The flag field helps us in the special case of IO = 0
					if((!p[i].IO && io[i] != 0) || p[i].flag) {
						//We don't need the process any more
						strcpy(p[i].state, "done");
						occupied = 0;
						//Helps us calculating the turnout time at the end
						p[i].END = lineNum-1;
						//Decrement the number of processes
						n--;					
					}
					//The special case if IO = 0
					else if(io[i] == 0 && !p[i].flag) {
						occupied = 1;
						p[i].CPU = cpu[i] - 1;
						strcpy(p[i].state, "running");
						p[i].flag = 1;
					}
					//If it doens't have the special case of IO = 0 and finished its CPU but hasn't entered the IO cycles 
					else {
						//Then block it
						strcpy(p[i].state, "blocked");
						occupied = 0;
					}			
				}
				//Otherwise, it still didn't finish the CPU cycles, so decrement it
				else
					p[i].CPU --;
				//This is to ensure that we don't print a process with stat "No" or "done"
				if(!strcmp(p[i].state, "running") || !strcmp(p[i].state, "blocked")) {
					printf("%d: %s ", p[i].PID, p[i].state);
					if(!strcmp(p[i].state, "running"))
						//Helps us calculating CPU Utilization at the end of the code
						busy++;
				}
					
			}
			//Second case is the if the process is in its IO cycles
			else if(!strcmp(p[i].state, "blocked")) {
				//If it has finished them
				if((p[i].IO-1 ) <= 0) {
					//Retrieve the value of its CPU cycles
					p[i].CPU = cpu[i];
					//This to ensure that after it finished its CPU cycles for the second time, it won't re-enter the IO cycles and so
					//forth
					p[i].flag = 1;
					//If it has finished the IO cycles and CPU is empty
					if(!occupied) {
						strcpy(p[i].state, "running");
						occupied = 1;
						p[i].CPU --;
					}
					//If it has finished the IO cycles and CPU is occupied	
					else {
						//Enqueue it in the ready queue
						strcpy(p[i].state, "ready");
						enqueue(q, p[i]);
					}					
				}
				//Otherwise it didn't finish its IO cycles yet
				else
					p[i].IO --;
				if(!strcmp(p[i].state, "running"))
						busy++;
				printf("%d: %s ", p[i].PID, p[i].state);
			}
			/**
			sz: We print the status of the processes in the Queue ONLY when we check for the last element in the queue, so
			we dequeue untill sz == queueSZ(q)
			**/
			else if (++sz == queueSZ(q)) {
				innerSZ = 0;
				//If we have a ready process and the CPU is empty
				if(queueSZ(q) && !occupied) {
					//Dequeue it and run it
					struct Process pr = dequeue(q);
					occupied = 1;
					strcpy(pr.state, "running");
					//Begin decrementing its CPU
					pr.CPU --;
					//But since we will change in the dequeued process not in the original process in the array itself					
					for(j = 0; j < rows;  j++) {
						//We search for it by PID and set its new state and CPU
						if(p[j].PID == pr.PID) {
							strcpy(p[j].state, pr.state);
							p[j].CPU = pr.CPU;
							break;							
						}
					}
					busy++;
					printf("%d: %s ", p[j].PID, p[j].state);
				}
				//innerSZ: In order to iterate throught the queue
				while(innerSZ < queueSZ(q)) {
					struct Process pr = dequeue(q);
					printf("%d: %s ", pr.PID, pr.state);
					//We don't want to mess the queue, so we enqueue the processes again
					enqueue(q, pr);
					innerSZ++;
				}
				sz = 0;
			}
		}
		printf("\n");
	}
	//Because the loop exists after n is equal to zero and we increment lineNum while printing, so the actual value of lineNum is increased
	//by 2 than its original value
	printf("Finishing time: %d", lineNum-2);
	printf("\nCPU utilization: %f\n",(busy*1.0)/(lineNum-1));
	sortPid(p, rows);
	for(i = 0; i < rows; i++){
		printf("Turnaround process %d: %d\n", i, p[i].END - p[i].ARRIV );
	} 
    return 0;
}
