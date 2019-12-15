# Heap Simulator

This project mocks the behavior of a heap using the C language. It simulates the following four features

1. Memory Allocation
2. Memory Deallocation
3. Memory Alocation Strategies
4. Memory Compaction

For full project requirement, please refer to this [link](https://docs.google.com/document/d/1DqC6x-DR7s7VsveSVOJKEosnSB010J5PXvQFBBkGel4/edit?usp=sharing).

## Table of Contents

- [Heap Simulator](#heap-simulator)
  - [Table of Contents](#table-of-contents)
  - [User Manual](#user-manual)
    - [Demo](#demo)
    - [Workflow](#workflow)
  - [Memory Allocation](#memory-allocation)
    - [Special Case: Allocated zone does not occupy all space](#special-case-allocated-zone-does-not-occupy-all-space)
    - [Case 1: freelist is allocated](#case-1-freelist-is-allocated)
    - [Case 2: freelist is not allocated](#case-2-freelist-is-not-allocated)
  - [Memory Deallocation](#memory-deallocation)
    - [Case 1: idx is the new freelist](#case-1-idx-is-the-new-freelist)
    - [Case 2: idx is not the new freelist](#case-2-idx-is-not-the-new-freelist)
      - [Subcase 1: there is no free zone except freelist](#subcase-1-there-is-no-free-zone-except-freelist)
      - [Subcase 2: there is more free zone besides freelist](#subcase-2-there-is-more-free-zone-besides-freelist)
        - [Implementation of int concat_list[]](#implementation-of-int-concat_list)
  - [Memory Allocation Strategies](#memory-allocation-strategies)
    - [1. first fit](#1-first-fit)
    - [2. best fit](#2-best-fit)
    - [3. worst fit](#3-worst-fit)
  - [Memory Compaction](#memory-compaction)

## User Manual

### Demo

To see how this works, simply execute the following commands

```shell
git clone https://github.com/ANPULI/Operating-Systems.git
cd Heap-Simulator-Canvas
make
```

The default behavior will use `first fit` as the allocation strategy, and the output should be the same as `output.txt`.

You can open `makefile` and comment/uncomment corresponding lines to see `best fit` and `worst fit`.

### Workflow

This section covers the use of every `c` file in `src/`. To make more changes, please refer to this section carefully.

| File Name  | Usage  |
|---|---|
| `memory-management.c`  | This is the most important file of the simulator. It implements all the features. You may want to refer to the following sections to understand what the code inside does  |
| `memory-simulation-run.c` | This file contains the simulation to be performed. You can play with different input to have different outputs |
| `memory-simulator.c` | This file contains the logical flow. You may don't want to modify this file.|

You can modify `src/memory-simulation-run.c` to make more changes.

## Memory Allocation

Given the `result` which is

```c
struct {
    int found;      //index in the heap array of the first cell of the target free zone
    int previous;   //index of the first cell of the free zone that precedes the target
};
```

We let `idx = result.found` and `prev = result.previous` to simplify coding.

### Special Case: Allocated zone does not occupy all space

We compare `size` with `heap[idx] - MIN_BLOCK_SIZE` first need to check if the allocated zone needs all space.

If `size < heap[idx] - MIN_BLOCK_SIZE`, we need to set the new free zone. If not, we can jump to the [normal operation](#normal-operation).

Then we need to check if `freelist == idx` to see if we need to reset `freelist`.

### Case 1: `freelist` is allocated

set `freelist` as `heap[idx+1]` (the next freezone)

### Case 2: `freelist` is not allocated

set `heap[prev+1]` as `heap[idx+1]` (the next freezone of the previous free zone should be the next free zone of this allocated zone)

![image](https://user-images.githubusercontent.com/26131764/70860306-2dda7d00-1f5b-11ea-9d05-94ca71b18c87.png)

## Memory Deallocation

We can divide into three cases.

Assume `idx` is the index of the zone to be freed, `size` is its size.

We add a function here as a helper

| Name                            | Usage                                                               |
|---------------------------------|---------------------------------------------------------------------|
| `void concat_freezone(int idx)` | check if `idx`'s next free zone is contiguous with it and merge if so |

### Case 1: `idx` is the new `freelist`

Two scenarios will lead to this case:

1. `idx` is left to `freelist`
2. `freelist` is -1 (there is no free zone)

In such case, what we do is to

1. point `heap[idx+1]` to `freelist`
2. set `freelist` to `idx`
3. merge if contiguous
   - `cancat_freezone(idx)`

### Case 2: `idx` is not the new `freelist`

We know that `idx` must be right to `freelist`, and `freelist != -1`. There are two subcases as follows.

#### Subcase 1: there is no free zone except `freelist`

We can tell that `heap[freelist+1] == -1` because there is no other free zone. We therefore

1. set `heap[idx+1]` to -1
2. point `heap[freelist+1]` to `idx`
3. merge if contiguous
   - `concat_freezone(idx)`

#### Subcase 2: there is more free zone besides `freelist`

We use a `concat_list` that stores the indexes of all free zones until the first free zone (including) after `idx`, the zone to be freed.

##### Implementation of `int concat_list[]`

We start from `i = freelist`: 

1. add `i` to `concat_list`
2. check if `heap[i+1] > idx`
   1. `heap[i+1] > idx`: add `heap[i+1]` to `concat_list`, exit the loop
   2. `heap[i+1] < idx`: add `heap[i+1]` to `concat_list`, continue the loop, set `i = heap[i+1]`

| Name                | Usage                                                          |
|---------------------|----------------------------------------------------------------|
| `int concat_list[]` | stores the indexes of all free zones that may be able to merge |

After we have the `concat_list`, we follow this procedure

1. set `heap[idx+1]` to the next free zone (-1 if no free zone after `idx`)
2. let `prev` be the first free zone that precedes `idx`, set `heap[prev+1]` to `idx`
3. starting from the back of `concat_list` in a loop, let the iterator be `j`
   - `concat_list(j)`

## Memory Allocation Strategies

### 1. first fit

loop through all free zones and return the first one that has enough size

```c
found = freelist
while size of found list < required size:
  found = next freezone
  prev = found
```

### 2. best fit

loop through all free zones and return the one where unused space is the minimum

We use an array that stores `freezone` struct of all who have enough size. Then we choose the one with the smallest size as `fz`.

### 3. worst fit

loop through all free zones and return the one where unused space is the maximum

We use an array that stores `freezone` struct of all who have enough size. Then we choose the one with the biggest size as `fz`.

## Memory Compaction

To eliminate external fragmentations, we fist add the following variables and functions for easier use.

| Name                                 | Usage                                        |
|--------------------------------------|----------------------------------------------|
| `char **allocations[]`               | an array that stores the pointers to all the pointers |
| `int nb_block`                       | records the number of pointers               |
| `void add_allocation(char** ptr)`    | add `ptr` to `allocations` ordering by index |
| `void remove_allocation(char** ptr)` | remove `ptr` from `allocations`              |

We have to call `add_allocation(&p)` and `remove_allocation(&p)` in the `memory-simulator-run` because we want to change the pointer itself, i.e. its address. There is no other way that we can have this pointer stored. 

Basically, this design keeps track of every pointer and stores them in an array, ordering by their indexes. When there is a new zone allocated to a pointer, we loop through `allocations` to find its place and insert it there. When we free, that pointer is removed from `allocations`. When we defragement the heap, things then get easier. We start from the first pointer in `allocations`, and move the corresponding data to the beginning of the heap. Then, we apply the same procedure to the following pointers. Last, all remaining space in the heap becomes a single free zone.