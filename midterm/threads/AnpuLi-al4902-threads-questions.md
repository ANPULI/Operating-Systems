# Threads Questions

- [Threads Questions](#threads-questions)
  - [Multiple Choices](#multiple-choices)
  - [Coding Problem](#coding-problem)

|      Field | Value      |
|-----------:|:-----------|
| First Name | Anpu       |
|  Last Name | Li         |
|     NYU ID | al4902     |
|       Date | 11/27/2019 |

## Multiple Choices

1. Which of the following is not shared among threads?
   a. Code
   b. Stack
   c. Data  
2. Which of the following is ture when comparing threads with child processes?
   a. Faster creation  
   b. Easier programming  
   c. Safer functions  

## Coding Problem

Consider the following program

```c
// initialization
int s;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* thread_push(void* arg);

int main() {

    s = 0;
    int i, tid[2], *pt_i;

    for (i = 0; i < 2; i++) {
        pt_i = (int*) malloc(sizeof(int));
        *pt_i = i;
        pthread_create(&tid[i], NULL, fun, (void*)pt_i);
    }

    for (i = 0; i < 2; i++) {
        pthread_join(tid[i], 0);
    }

    return 0;
}

void* thread_push(void* arg) {
    int i = *(int*) arg;
    if (i == 0) {
        pthread_mutex_lock(&mutex);
        s *= 2;
        pthread_mutex_unlock(&mutex);
    } else {
        pthread_mutex_lock(&mutex);
        s += 10;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

```

`s` is the saving account. Add code to ensure `s+=10` happens before `s*=2`.
