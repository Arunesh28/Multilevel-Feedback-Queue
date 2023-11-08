# Multilevel-Feedback-Queue

### Multi Level feedback queue Implementation
#### Aim

The objective of this lab assignment is to implement commonly used scheduling algorithms (FCFS, SJF, RR) as discussed in class, in a system that supports multi-level queues with feedback.

#### Requirements
 - The system has four queues: Q4 is highest priority and Q1 has lowest priority.
 - The CPU schedules from Qx only if there are no processes in Q(x+1).
 - Q4 uses RR; Q3 uses SJF; Q2 uses SJF; Q1 uses FCFS. All algorithms will be in non-preemptive mode; A process under RR will execute under the specified timeslice runs out or it completes its burst.
 - After a process spends some specified threshold time in the system, the system moves to the immediate higher queue. Processes in Q1 cannot obviously move to a higher level.
 - Here is a brief specification; you can make additional assumptions as needed. All units are taken to be in milli-seconds.

#### The command-line arguments are:

    Q: time quantum for Robin-robin scheme (a number between 10 and 20)

    T: time threshold for moving a process to the next level (a number between 100 and 50000)

    F: input filename

    P: output filename


The input file (specified by the command line), has a set of entries, one entry per line, as shown below:

    1 3 0 100
    2 4 0 70
    3 2 10 35 
    4 1 20 40
    ...

 - The specified fields are in the following order: ID, Initial Queue Level,Arrival time (ms),CPU Burst time (ms).
 - What to Measure: For each process, measure the time between completion time and the time of addition to the first queue it was assigned to. For the system, measure the throughput (no. of processes completed per unit time).
 - What to Output: For each process, upon its completion, print the following in the output file:
```
ID: xy; Orig. Level: ab; Final Level: cd; Comp. Time(ms): ;TAT (ms):
```
At the end of running all processes specified in the file, print the following information in the output file:
```
Mean Turnaround time:  abc.de (ms); Throughput: xyz.gh processes/sec
```
#### Steps to run
 - Below is an example to run :
  ```
    make all
    ./mlfq 10 100 input_file.txt ./output/output_file.txt > run.log
  ```
 - Run the below command to remove output, .exe etc.
```
    make clean
```

