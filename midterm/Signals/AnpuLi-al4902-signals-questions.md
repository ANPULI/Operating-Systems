# Signals Questions

### Name: Anpu Li

### NetID: al4902

## Multiple Choices

1. When receiving the following signals, which one, assuming default handler, will not cause the process to terminate? (Choose all that apply)  
   a. SIG_USR1  
   b. SIG_KILL  
   c. SIG_STOP  
   d. SIG_ALRM  
   e. SIG_CHLD
2. After fork(), what attributes of the parent will not be inherited by the child?  
   a. pending signals  
   b. signal masks  
   c. signal handlers

## Tracing Problem

Consider the following code, give the correct sequence of signals sent to the process to let it end normally.

*`signal(sig, handler)` sets the action of `sig` to `handler`.*

```c
int i;

void sig_hand(int sig) {
    if (SIGINT == sig) {
        i += 2;
    } else if (SIGUSR1 == sig) {
        i -= 2;
    } else if (SIGUSR2 == sig) {
        i *= 2;
    }
}

int main(int argc, char** argv) {

    i = 1;
    sigset_t sig_proc;
    signal(SIGINT, sig_hand);
    signal(SIGUSR1, sig_hand);
    signal(SIGUSR2, sig_hand);

    printf("Please help me out of the infinite loop!\n");
    while (i) {
        
    }
    printf("Thank you! Now I can safely go now.\n");

    return 0;
}
```