# Heap Simulator

Project requirement: [link](https://docs.google.com/document/d/1DqC6x-DR7s7VsveSVOJKEosnSB010J5PXvQFBBkGel4/edit?usp=sharing).

## Memory Allocation

Given the `result` which is

```c
struct {
    int found;      //index in the heap array of the first cell of the target free zone
    int previous;   //index of the first cell of the free zone that precedes the target
};
```

We let `idx = result.found` and `prev = result.previous` to simplify coding.

We need to check if freelist is idx to see if we need to reset freelist.

### `freelist` is allocated

set `freelist` as `heap[idx+1]` (the next freezone)

### `freelist` is not allocated

set `heap[prev+1]` as `heap[idx+1]`

the next freezone of the previous free should be the next free zone this allocated zone.

make a graph here

### Special Case: Allocated zone does not occupy all space

We compare `size` with `heap[idx] - MIN_BLOCK_SIZE` first need to check if the allocated zone needs all space.

If `size < heap[idx] - MIN_BLOCK_SIZE`, we need to set the new free zone. If not, we can jump to the [normal operation](#normal-operation).

```c
set the size of the new free zone
set
```

### Normal Operation

1. if not occupy all space (size <= heap[idx] - MIN_BLOCK_SIZE) (the minimum is 2)
   - set new free zone
   - heap[idx+1] = idx + size + 1
   - more code going
   - goto 2
2. if occupy all space
   - if (prev == -1)     freelist == idx
     - freelist = heap[idx+1]
   - else
     - heap[prev+1] = heap[idx+1]

## Memory Deallocation

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