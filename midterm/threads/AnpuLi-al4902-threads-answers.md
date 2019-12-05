# Threads Answers

- [Threads Answers](#threads-answers)
  - [Multiple Choices](#multiple-choices)
  - [Coding Problem](#coding-problem)

|      Field | Value      |
|-----------:|:-----------|
| First Name | Anpu       |
|  Last Name | Li         |
|     NYU ID | al4902     |
|       Date | 11/27/2019 |

## Multiple Choices

1. b
2. a

## Coding Problem

```c
// initialization
int s, flag;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void* thread_push(void* arg);

int main() {

    s = flag = 0;
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
        while (flag != 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        s *= 2;
        pthread_mutex_unlock(&mutex);
    } else {
        pthread_mutex_lock(&mutex);
        s += 10;
        flag ++;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

```