
First Name:

Last Name:

NYU ID:

=======================================================================================
Files submitted:

src:
	fifo_list.c: Question 6
	fifo_test.c: Question 6
	list_impl.c: Question 4, Question 5, Question 6
	stack_array.c: Question 1
	stack_list.c: Question 5
	stack_test.c: Question 1, Question2, Question 3, Question 5

include:
	fifo.h: Question 6
	list.h: Question 4, Question 5, Question 6
	stack.h: Question 1, Question 2, Question 3 and Question 5


=======================================================================================
Compilation rules in the Makefile:

# A few examples below
# make libstack: 	generates and registers the array-based stack library (lib/libstack.a) (Q2)
# make stacksize: 	recompiles the stack with a new value for STACK_SIZE as a compilation directive and runs the stack test (Q3)
# make newlibstack:	recompiles the stack, implemented as a double-linked list, then integrates it in an update of libstack.a (Q5)
# make Q5:		uses newlibstack to recompile the binary executable (bin/stack_test), then runs it (Q5)
# make libfifo: 	compiles a dynamic queue implemented as a double-linked list, then integrates it in a library (lib/libfifo.a) (Q6)


=======================================================================================
Comments:
I spent really a long time debugging Q5. The problem of segmentation faults kept haunting me no matter what I did to get rid of it. 
I followed stack_array.c. But after hours I found I must use malloc to give the list a space. It makes sense but I don't know why in 
stack_array.c it is not necessary. And also if I directly call functions in list_impl.c from stack_test.c, it works fine. But if I have 
a middle function in stack_test.c that calls list_impl.c, the segmentation faults recur.


=======================================================================================
Textual answers: 

Question 7:
	In list_impl.c and list.h, we add an int variable "count" that represents the size of the list. In init, we set count=0. When we insert, count++ and when we extract, count--.
	And then we can also determine if the list is empty/full by checking count.
	