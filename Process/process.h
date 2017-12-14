#ifndef PROCESS_INCLUDED
#define PROCESS_INCLUDED
struct Process {
    int PID, CPU, IO, ARRIV, flag;
	char *state; 
};
void processInit(struct Process *p);

#endif // PROCESS_INCLUDED
