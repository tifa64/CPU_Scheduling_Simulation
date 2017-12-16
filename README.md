# CPU_Scheduling_Simulation
+ Simulates  two  operating  system  scheduling algorithms:  First  Come  First  Served  (FCFS)  and  Round Robin  (RR)
---
## Code Structure 
_First come first serve scheduling algorithm:_
  -	Reading input:
    + The input processes are read from a file line by line, then storing the input in an array of structure.
    
  - Sorting input:
    + After reading the input we sort it based upon the arrival time of each process.

  - Algorithm used:
    + In every CPU cycle we have to check if there is a new coming process, if the CPU is empty this process will execute immediately if not it will be added to the ready queue.
    + If the CPU time of the current process ended either it will be blocked for input/output request or will be added to the ready queue, after the process finishes its i/o time it will be added to the ready queue.
    + Check if the CPU is empty and there Is processes in the ready queue, they will be scheduled to the CPU.
