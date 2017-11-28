#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define ITERATION 1000000
#define MAX 10

pthread_mutex_t     mutex;
pthread_cond_t      isAvailable;
pthread_cond_t      isEmpty;

int array[MAX];
int id = 0;

void initialization() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&isAvailable, NULL);
    pthread_cond_init(&isEmpty, NULL);
    
    for (int i = 0; i < MAX; i++) {
        array[i] = 0;
    }
}

void deinit() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&isAvailable);
    pthread_cond_destroy(&isEmpty);
}

void showArrayStat() {
    printf("[ ");
    for (int i = 0; i < MAX; i++) {
        printf(" %i ", array[i]);
    }
    printf(" ]\n");
}

void konzumuj(void data) {
    for (int i = 0; i < ITERATION; i++) {
        pthread_mutex_lock(&mutex);
        
        while(id == 0) {
            pthread_cond_wait(&isAvailable, &mutex);
        }
        
        id --;
        array[id] = 0;
        
        printf("Konzumacia => ");
        showArrayStat();
        
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&isEmpty);
        sleep(0.2);
    }
    pthread_exit(NULL);
}

void produkuj(void data) {
    for(int i = 0; i < ITERATION; i++) {
        pthread_mutex_lock(&mutex);
        
        while(id >= 9) {
            pthread_cond_wait(&isEmpty, &mutex);
        }
    
        array[id] = 1;
        id ++;
        
        printf("Vyroba => ");
        showArrayStat();
        
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&isAvailable);
        
        sleep(0.1);
    }
    pthread_exit(NULL);
}


int main() {
    initialization();

    pthread_t konzument;
    pthread_t producent;
    
    pthread_create(&konzument, NULL, konzumuj, NULL);
    pthread_create(&producent, NULL, produkuj, NULL);
    
    pthread_join(konzument, NULL);
    pthread_join(producent, NULL);
    deinit();
    
    return 0;
}