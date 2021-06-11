#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>

union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};

#define PATH "/mnt/d/국민대2021-1"

typedef struct __counter_t {
    int value;
    key_t key;
    int semid;
    union semun arg;
} counter_t;

unsigned int loop_cnt;
counter_t counter;

void init(counter_t *c, char *argv[]) {
    c->value = 0;
    c->key = ftok(PATH, 'z');
    c->semid = semget(c->key, 1, 0600 | IPC_CREAT);
    c->arg.val = 1;
    semctl(c->semid, 0, SETVAL, c->arg);

    if (c->key < 0){
	    perror(argv[0]);
	    exit(1);
    }
    if(c->semid < 0){
	    perror(argv[0]);
	    exit(1);
    }
}

void increment(counter_t *c) {
    struct sembuf s;
    s.sem_num = 0;
    s.sem_op = -1;
    s.sem_flg = 0;
    semop(c->semid, &s, 1);
    c->value++;
    s.sem_num = 0;
    s.sem_op = 1;
    s.sem_flg = 0;
    semop(c->semid, &s, 1);
}

void decrement(counter_t *c) {
    struct sembuf s;
    s.sem_num = 0;
    s.sem_op = -1;
    s.sem_flg = 0;
    semop(c->semid, &s, 1);
    c->value--;
    s.sem_num = 0;
    s.sem_op = 1;
    s.sem_flg = 0;
    semop(c->semid, &s, 1);
}

int get(counter_t *c) {
    struct sembuf s;
    s.sem_num = 0;
    s.sem_op = -1;
    s.sem_flg= 0;
    semop(c->semid, &s, 1);
    int rc = c->value;
    s.sem_num = 0;
    s.sem_op = 1;
    s.sem_flg = 0;
    semop(c->semid, &s, 1);
    return rc;
}

void *mythread(void *arg)
{
    char *letter = arg;
    int i;

    printf("%s: begin\n", letter);
    for (i = 0; i < loop_cnt; i++) {
        increment(&counter);
    }
    printf("%s: done\n", letter);
    return NULL;
}
                                                                             
int main(int argc, char *argv[])
{                    
    
    clock_t start, end;    

    loop_cnt = atoi(argv[1]);
    
    start = clock();

    init(&counter, argv);

    pthread_t p1, p2;
    printf("main: begin [counter = %d]\n", get(&counter));
    pthread_create(&p1, NULL, mythread, "A"); 
    pthread_create(&p2, NULL, mythread, "B");
    // join waits for the threads to finish
    pthread_join(p1, NULL); 
    pthread_join(p2, NULL); 
    printf("main: done [counter: %d] [should be: %d]\n", get(&counter), loop_cnt * 2);
    
    end = clock();
    printf("Performance: %fsec\n", (double)(end-start)/CLOCKS_PER_SEC);

    return 0;
}
