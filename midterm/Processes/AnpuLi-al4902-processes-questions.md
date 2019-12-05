<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>
<script type="text/x-mathjax-config">
    MathJax.Hub.Config({ tex2jax: {inlineMath: [['$', '$']]}, messageStyle: "none" });
</script>

# Processes Questions

### Name: Anpu Li

### NetID: al4902

## Multiple Choices

1. Which of the followings are the three segments of a process? (Choose all correct answers)  
   a) code  
   b) data  
   c) stack  
   d) heap
2. Suppose that $p$ is an 8-byte pointer located at **00EF** that points to a 16-byte struct, and that $q$, also an 8-byte pointer, points to a 24-byte struct. Given we allocate the memory properly, what will be $q's$ address if $q$ is assigned after $p$? How about before $p$?  
   a) 00DF, 0107  
   b) 00FF, 00D7  
   c) 00D7, 00FF  
   d) 0107, 00DF
3. If a dynamically allocated variable gets changed by the child after fork and parent waits for his child, will parent also receive the change?  
   No

## Tracing Problem

What will be the result of the following code?

```c
int main() {
    int i, a, b, p;
    a = 1; b = 0;

    for (i = 0; i < N; i++) {
        p = fork();
        if ( p == -1) {
            exit(1);
        } else if (p > 0) {
            break;
        } else if (p == 0) {
            printf("level %d: %d\n", i+1, a);
            a += b;
            b = a - b;
        }
    }

    return 0;
}
```

## Code Modification

Modify the code so that each process waits for its child.