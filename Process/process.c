#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"


void processInit(struct Process *p) {
	p->state = (char *) malloc(100);
	strcpy(p->state, "No");
	p->flag = 0;
}
