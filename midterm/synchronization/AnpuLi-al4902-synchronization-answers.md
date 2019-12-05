# Synchronization Answers

- [Synchronization Answers](#synchronization-answers)
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

1. b, c
2. c

## Tracing Problem

|   | Person 1      | Person 2      |   |
|---|---------------|---------------|---|
| 1 | P(spoons)     | P(residual)   | 2 |
| 3 | P(chopsticks) | P(household)  | 5 |
| 4 | V(spoons)     | V(residual)   | 6 |
| 7 | P(household)  | P(chopsticks) | 8 |
|   | V(chopsticks) | V(household)  |   |
|   | P(residual)   | P(spoons)     |   |
|   | V(household)  | V(chopsticks) |   |
|   | V(residual)   | V(spoons)     |   |

The numbers at the leftmost/rightmost are the execution order. Person 1 is standing at chopsticks, waiting for person 2 to finish. Person 2 is standing at household waste, waiting for person 1 to finish. This creates a deadlock.

Simple solution: People be kind and make a line at their next disposal spots. That is, after they finish one task, they directly move to the next spot and line up for it instead of staying still at the original spot to wait for an empty place at the next spot.

## Coding Problem

```c
// initialization
mx = 1;

// Process 1
while (1) {
    P(mx);
    enter_CS();
    exit_CS();
    V(mx);
}

// Process 2
while (1) {
    P(mx);
    enter_CS();
    exit_CS();
    V(mx);
}
```