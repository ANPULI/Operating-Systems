#include <os-memory.h>
#include <string.h>

void run_simulation() {
    
    printf("Begin simulation\n");
    
    char *p1, *p2, *p3, *p4;
    p1 = (char*) heap_malloc(10);
    strcpy(p1, "v1");
    // print_heap();
    p2 = (char*) heap_malloc(9);
    strcpy(p2, "v2");
    p3 = (char*) heap_malloc(5);
    strcpy(p3, "v3");
    print_heap();

    
    heap_free(p2);
    heap_free(p1);
    print_heap();

    p4 = (char*) heap_malloc(7);
    strcpy(p4, "system");
    print_heap();

    heap_free(p3);
    print_heap();
    printf("%s\n", p4);

    
    printf("End simulation\n");
}

