#include <os-memory.h>
#include <string.h>

void run_simulation() {
    
    printf("Begin simulation\n");
    
    char *p1, *p2, *p3, *p4;
    p1 = (char*) heap_malloc(10);
    add_allocation(&p1);
    strcpy(p1, "v1");
    // print_heap();
    p2 = (char*) heap_malloc(9);
    add_allocation(&p2);
    strcpy(p2, "v2");
    p3 = (char*) heap_malloc(5);
    add_allocation(&p3);
    print_allocations();
    strcpy(p3, "v3");
    print_heap();

    
    remove_allocation(&p2);
    heap_free(p2);
    remove_allocation(&p1);
    heap_free(p1);
    print_allocations();
    print_heap();

    // heap_defrag();
    // print_heap();

    p4 = (char*) heap_malloc(7);
    add_allocation(&p4);
    print_allocations();
    strcpy(p4, "system");
    print_heap();

    remove_allocation(&p3);
    heap_free(p3);
    print_allocations();
    print_heap();
    printf("%s\n", p4);

    // heap_defrag();
    // print_heap();
    
    printf("End simulation\n");
}

