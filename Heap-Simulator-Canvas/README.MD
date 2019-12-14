# Heap Simulator

Project requirement: [link](https://docs.google.com/document/d/1DqC6x-DR7s7VsveSVOJKEosnSB010J5PXvQFBBkGel4/edit?usp=sharing).

- [Heap Simulator](#heap-simulator)
  - [Memory Allocation](#memory-allocation)
    - [Special Case: Allocated zone does not occupy all space](#special-case-allocated-zone-does-not-occupy-all-space)
    - [Case 1: freelist is allocated](#case-1-freelist-is-allocated)
    - [Case 1: freelist is not allocated](#case-1-freelist-is-not-allocated)
  - [Memory Deallocation](#memory-deallocation)
    - [Case 1: idx is the new freelist](#case-1-idx-is-the-new-freelist)
    - [Case 2: idx is not the new freelist](#case-2-idx-is-not-the-new-freelist)
      - [Subcase 1: there is no free zone except freelist](#subcase-1-there-is-no-free-zone-except-freelist)
      - [Subcase 2: there is more free zone besides freelist](#subcase-2-there-is-more-free-zone-besides-freelist)
  - [rdm](#rdm)
  - [Memory Allocation Strategies](#memory-allocation-strategies)
    - [1. first fit](#1-first-fit)
    - [2. best fit](#2-best-fit)
    - [3. worst fit](#3-worst-fit)
  - [Memory Compaction](#memory-compaction)

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

### Case 1: `freelist` is not allocated

set `heap[prev+1]` as `heap[idx+1]` (the next freezone of the previous free zone should be the next free zone of this allocated zone)


:warning: TODO: make a graph

## Memory Deallocation

We can divide into three cases.

Assume `idx` is the index of the zone to be freed, `size` is its size.

### Case 1: `idx` is the new `freelist`

Two scenarios will lead to this case:

1. `idx` is left to `freelist`
2. `freelist` is -1 (there is no free zone)

In such case, what we do is to

1. point `heap[idx+1]` to `freelist`
2. set `freelist` to `idx`
3. merge if contiguous

### Case 2: `idx` is not the new `freelist`

We know that `idx` must be right to `freelist`, and `freelist` is not -1. There are two subcases as follows.

#### Subcase 1: there is no free zone except `freelist`

We can tell that `heap[freelist+1]` is -1 because there is no other free zone. We therefore

1. set `heap[idx+1]` to -1
2. point `heap[freelist+1]` to `idx`
3. merge if contiguous

#### Subcase 2: there is more free zone besides `freelist`

We use a `concat_list` that stores the indexes of all free zones until the first free zone (including) after `idx`, the zone to be freed.

1. set `heap[idx+1]` to the next free zone (-1 if no free zone after `idx`)
2. point the zone before `idx` to `idx`
3. merge if contiguous, starting from the back of `concat_list` in a loop

## rdm


1. find next free zone
   - `heap[idx+1]` should be set to the index of next free zone
   - use `freelist` to find the next free zone
   - freelist always the leftmost
   - if (idx < freelist) or (freelist == -1) --> you are the freelist
     - `heap[idx+1] = freelist`
   - else
     - loop through the heap using free list to find the next freezone
   - if the freezones are successive
     - concat


current status

it is very complicated to do a backward freezone concatenation

a sorted-list? not very good idea, too costly

i use a list that stores every freelist and do it backwards.

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

| Name                                | Usage                                 |
|-------------------------------------|---------------------------------------|
| `char *allocations[]`               | an array that stores all the pointers |
| `int nb_block`                      | records the number of pointers        |
| `void remove_allocation(char* ptr)` | remove `ptr` from `allocations`       |