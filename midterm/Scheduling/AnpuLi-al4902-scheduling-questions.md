# Scheduling Questions

### Name: Anpu Li

### NetID: al4902

## Multiple Choices

1. Which of the following algorithms is subject to a famine?  
   a. SJF  
   b. FIFO  
   c. RR
2. Which one is true of SJF?  
   a. Minimal average waiting time  
   b. No risk of famine  
   c. Easy to get CPU time

## Scheduling Problem

Timesharing with dynamic priorities

Consider a multilevel feedback queue strategy where every new task gets inserted at the end of the queue of highest priority. If a task is suspended for I/O, it gets inserted at the back of the queue that is one priority level lower than the queue it got elected from. If a task reaches the quantum limit, it gets inserted at the back of its current queue.

Consider the following task insertion model:

|  | CPU time | Insertion rate |
|----|----------|----------------|
| T1 | 15 ms | every 150 ms |
| T2 | 200 ms | every 300 ms |
| T3 | 350 ms | -    |

Assume that at date d = 0, tasks T1.0, T2.0, and T3.0 enter the system in that order. Whenever a task T1.x enters the system at the same date as a task T2.y, T1.x always precedes T2.y in the queue.

Let the quantum Q = 100ms. Give the output till T3 ends.
