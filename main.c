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
void sort(struct Process* p, int n) {
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
int main()
{
    struct Queue *q = initialize(10);
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
	sort(p, rows);
	for(i = 0; i < rows;  i++) {
	printf("%d %d %d %d %s\n", p[i].PID, p[i].CPU, p[i].IO, p[i].ARRIV, p[i].state);
	cpu[i] = p[i].CPU;
	io[i] = p[i].IO;
}
	int lineNum = 0, n = rows, occupied = 0, k = 0;
	i = 0, j = 0;
	while(k++ < 20) {
		for(i = 0; i < n ; i++) {
			if(strcmp(p[i].state, "done") == 0)
				continue;
			if(p[i].ARRIV == i && !strcmp(p[i].state, "No") ) {
				if(occupied) {
					strcpy(p[j].state, "ready");
					enqueue(q, p[j]);
					continue;
                }
				for(j = 0; j < n; j++) {
					if(i == j)
						continue;
					if(p[j].ARRIV == i && !strcmp(p[j].state, "No")) {
						strcpy(p[j].state, "ready");
						enqueue(q, p[j]);
					}
				}
				strcpy(p[i].state, "running");
				occupied = 1;
			}
		}
		printf("%d ", lineNum++);
		if(queueSZ(q)) {
			struct Process pr = dequeue(q);
			//printf("blaaaaaa %d: %s", pr.PID, pr.state);
		}
		for(i = 0; i < rows;  i++) {
			if(!strcmp(p[i].state, "No") || !strcmp(p[i].state, "done")) {
				continue;
			}
			printf("%d: %s", p[i].PID, p[i].state);
			p[i].CPU --;
			if(p[i].CPU <= 0) {
				if(!p[i].IO) {
					if(p[i].flag == 1) {
						strcpy(p[i].state, "done");
						occupied = 0;
						n--;					
					}
					else {
						p[i].flag = 1;
						p[i].CPU = cpu[i];
						if(io[i] == 0)
							strcpy(p[i].state, "running");
						else {
							strcpy(p[i].state, "ready");
							enqueue(q, p[j]);
						}
					}			
				}
				else {
					strcpy(p[i].state, "blocked");
					p[i].IO --;	
				}
			}
		}
		printf("\n");
	}
    return 0;
}
