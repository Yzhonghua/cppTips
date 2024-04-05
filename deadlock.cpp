#include <pthread.h>
#include <iostream>
#include <unistd.h>

pthread_mutex_t mutexA;
pthread_mutex_t mutexB;
using namespace std;

void *funcA(void *) {
    pthread_mutex_lock(&mutexA);
    sleep(1);
    cout << "A sleeping" << endl;
    pthread_mutex_lock(&mutexB);
    cout << "A got" << endl;
    pthread_mutex_unlock(&mutexB);
    pthread_mutex_unlock(&mutexA);
    return nullptr;
}

void *funcB(void *) {
    pthread_mutex_lock(&mutexB);
    sleep(1);
    cout << "B sleeping" << endl;
    pthread_mutex_lock(&mutexA);
    cout << "B got" << endl;
    pthread_mutex_unlock(&mutexA);
    pthread_mutex_unlock(&mutexB);
    return nullptr;
}



int main() {
    pthread_mutex_init(&mutexA, nullptr);
    pthread_mutex_init(&mutexB, nullptr);

    pthread_t threadA, threadB;
    pthread_create(&threadA, nullptr, funcA, nullptr);
    pthread_create(&threadB, nullptr, funcB, nullptr);

    pthread_join(threadA, nullptr);
    pthread_join(threadB, nullptr);

    pthread_mutex_destroy(&mutexA);
    pthread_mutex_destroy(&mutexB);

    cout << "END" << endl;
    return 0;
}
