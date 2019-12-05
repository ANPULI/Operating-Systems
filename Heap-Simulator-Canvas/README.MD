malloc

1. if not occupy all space (size <= heap[idx] - 2)
   - set new free zone
   - heap[idx+1] = idx + size + 1
   - more code going
   - goto 2
2. if occupy all space
   - if (prev == -1)     freelist == idx
     - freelist = heap[idx+1]
   - else
     - heap[prev+1] = heap[idx+1]

Free

1. clean all elements 
   - `size = heap[idx]`
   - `heap[idx+1] ... heap[idx+size]` set as 0
2. find next free zone
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