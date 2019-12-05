# Synchronization Questions

- [Synchronization Questions](#synchronization-questions)
  - [Multiple Choices](#multiple-choices)
  - [Tracing Problem](#tracing-problem)
  - [Coding Problem](#coding-problem)

|      Field | Value      |
|-----------:|:-----------|
| First Name | Anpu       |
|  Last Name | Li         |
|     NYU ID | al4902     |
|       Date | 11/17/2019 |

## Multiple Choices

1. What are the disadvantages of a spinlock? (Choose all)  
   a. It is difficult to implement  
   b. It requires busy waiting  
   c. It is not fully reliable  
2. _____ refers to the ability of multiple process (or threads) to share code, resources or data in such a way that only one process has access to shared object at a time.
   a. Starvation  
   b. Deadlock  
   c. Mutual Exclution  
   d. Synchronization

## Tracing Problem

NYU Shanghai cafeteria adopted the garbage sorting policy. In addition to sorting the tableware, now people need to sort the waste. Assume a standard procedure will be

> put spoons → put chopsticks → dispose household waste → dispose residual waste

Owing to the limited space, every action can only be done by one person simultaneouly. Also, assume there is only a single line across the four spots.
For example, if a person has finished with spoons, he will only release the space when he occpuies the next spot. If the next destination is already occupied, one must wait for that person to leave that space before he proceeds.

However, suddenly there is one person who goes on the wrong route and decides to do everything in a reverse order. Describe how a deadlock may occur in this situation. And propose a solution to it.

| Normal        |   | Reverse       |
|---------------|---|---------------|
| P(spoons)     |   | P(residual)   |
| P(chopsticks) |   | P(household)  |
| V(spoons)     |   | V(residual)   |
| P(household)  |   | P(chopsticks) |
| V(chopsticks) |   | V(household)  |
| P(residual)   |   | P(spoons)     |
| V(household)  |   | V(chopsticks) |
| V(residual)   |   | V(spoons)     |

## Coding Problem

Consider the following program

```c
// initialization
w1 = 0; w2 = 0

// Process 1
while (1) {
    w1 = 1;
    while (w1 == w2);
    enter_CS();
    exit_CS();
    w1 = 0;
}

// Process 2
while (1) {
    w2 = 1;
    while (w1 != w2);
    enter_CS();
    exit_CS();
    w2 = 0;
}
```

This program ensures mutual exclution and prevents deadlock. Please use semaphores to achieve same effect.
