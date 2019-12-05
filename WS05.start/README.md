# Assignment 5 - Semaphores

|      Field | Value      |
|-----------:|:-----------|
| First Name | Anpu       |
|  Last Name | Li         |
|     NYU ID | al4902     |
|       Date | 11/14/2019 |

- [Assignment 5 - Semaphores](#assignment-5---semaphores)
  - [Files submitted](#files-submitted)
    - [src](#src)
    - [include](#include)
  - [Targets in the makefile](#targets-in-the-makefile)
  - [Comments](#comments)
    - [Question 1 - Multi-currency conversion server](#question-1---multi-currency-conversion-server)
      - [Communication](#communication)
      - [Server memory design](#server-memory-design)
      - [Client memory design](#client-memory-design)
      - [Workflow](#workflow)
      - [Conversion](#conversion)
    - [Question 2 - Dining Philosophers](#question-2---dining-philosophers)
      - [Question](#question)
      - [Version 1 - take 2 chopsticks per round](#version-1---take-2-chopsticks-per-round)
      - [Version 2 - take 1 chopstick only per round](#version-2---take-1-chopstick-only-per-round)
      - [Version 3 - take 1 chopstick only per round](#version-3---take-1-chopstick-only-per-round)

## Files submitted

### src

| File                      | Question   |
|---------------------------|------------|
| `conversion_client.c`     | Question 1 |
| `conversion_server.c`     | Question 1 |
| `converters.c`            | Question 1 |
| `dining_philosophers_1.c` | Question 2 |
| `dining_philosophers_2.c` | Question 2 |
| `dining_philosophers_3.c` | Question 2 |

### include

| File           | Question   |
|----------------|------------|
| `converters.h` | Question 1 |

## Targets in the makefile

| TARGET           | USAGE                        |
|------------------|------------------------------|
| `make Q1_server` | make a server for question 1 |
| `make Q1_client` | make a client for question 1 |
| `make Q2_1`      | version 1 of question 2      |
| `make Q2_2`      | version 2 of question 2      |
| `make Q2_3`      | version 3 of question 2      |

## Comments

### Question 1 - Multi-currency conversion server

#### Communication

We are going to use shared memory for communication. Since the arguments passed when creating a server is only `server_id`, the server does not know anything about the client beforehand. However, the client has the `server_id` so that it can access the shared memory of server if the address is align with `server_id`. To this end, we name the shared memory of server and client as `[server_id]_shm` and `[client_id]_shm` respectively. To prevent two clients sending request simultaneouly, we set up a `mutex` initialized as 1. The client calls `P(mutex)` before sending request and the server calls `V(mutex)` after it solves the request.

#### Server memory design

The server requires:

1. `client_id`  -->  output address
2. `currency`  --> source currency
3. `amount`  --> source amount

Therefore we design `server_shm` as

```c
struct {
    char client_id[32];
    char currency[4];
    float amount;
}
```

so that the client can put the required information here

#### Client memory design

We design `client_shm` as

```c
struct {
    float vals[NB_CONVERTERS];
}
```

that shores the conversion results respectively for each currency


#### Workflow

The server serves for one client at one time. The model is as follows

`s_client_id = 0; s_server = 0; mutex = 1`

| client         | server        |
|----------------|---------------|
| P(mutex)       | P(s_server)   |
| put info       | get info      |
| V(s_server)    | put result    |
| P(s_client_id) | V(s_client_id)|
| display result | V(mutex)      |

- `put info`: client writes the request into the server memory
- `get info`: server retrieves the request from the server memory
- `put result`: server writes the result into the client memory
- `display result`: client displays the result from the client memory

Note that for each client we have a specific `s_client_id`. The server can know this because the client puts its id info into server's shared memory.

#### Conversion

Before the server starts accepting requests, it creates `N` child processes, each responsible for the conversion of one currency. Upon creation, the child process suspends itself with `SIGUSR1`. When the server receives a request, it sends signals to the child processes. The child processes inherit the server memory so they can look into it and writes the conversion resultsinto the specific location in the child memory.

### Question 2 - Dining Philosophers

:warning:**NB**: In the code, I define a new status of chopsticks as `USING`, indicating that the person with the chopsticks are actually eating, in contrast with `TAKEN` where the person is just taking that chopstick. I also have another status for philosohper `SEMI`, indicating that he has one chopstick on hand. 

#### Question

A deadlock will happen if everyone picks up the chopstick left to him/her, where every chopstick is taken and everyone is waiting for his/her right neighbor to put the chopstick down.

#### Version 1 - take 2 chopsticks per round

In version 1, the implementation is pretty naive and is covered in class. I would call it: take two chopsticks per round.

```
P(Mx)
think
check chopsticks status
if both are not taken:
    take chopsticks
V(Mx)

if have two chopsticks:
    eat

if eating:
    P(Mx)
    put chopsticks down
    V(Mx)
```

#### Version 2 - take 1 chopstick only per round

In version 2, the philosopher only takes one chopstick per round. To prevent deadlock in this case, we enforce that the person may only take a chopstick when both chopsticks to his/her left and right are not taken by other people.

```
P(Mx)
think
check chopsticks status
if no chopsticks on hand:
    if both are not taken:
        take the left chopstick
if has one chopstick:
    if right is not taken:
        take the right chopstick
V(Mx)

if have two chopsticks:
    eat

if eating:
    P(Mx)
    put chopsticks down
    V(Mx)
```

#### Version 3 - take 1 chopstick only per round

The difference between version 2 and version 3 is that in version three, when the philosopher wants to pick up his left chopstisk, not only does he/she check left and right, but also left's left.

```
P(Mx)
think
check chopsticks status
if no chopsticks on hand:
    if both are not taken:
        if left's left is not taken
            take the left chopstick
if has one chopstick:
    if right is not taken:
        take the right chopstick
V(Mx)

if have two chopsticks:
    eat

if eating:
    P(Mx)
    put chopsticks down
    V(Mx)
```
