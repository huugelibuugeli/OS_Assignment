#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
using namespace std;

#define SHM_NAME "/table_shm"

const int TABLE_SIZE = 2;

int main() {
    int shm_fd;
    int* table;

    // Open existing shared memory
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        cerr << "Error opening shared memory: " << strerror(errno) << "\n";
        exit(1);
    }

    table = static_cast<int*>(mmap(NULL, TABLE_SIZE * sizeof(int),
                                   PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (table == MAP_FAILED) {
        cerr << "Error mapping shared memory: " << strerror(errno) << "\n";
        exit(1);
    }

    // Open existing semaphores
    sem_t* empty = sem_open("/empty_sem", 0);
    sem_t* full  = sem_open("/full_sem",  0);
    sem_t* mutex = sem_open("/mutex_sem", 0);

    if (empty == SEM_FAILED || full == SEM_FAILED || mutex == SEM_FAILED) {
        cerr << "Error opening semaphores: " << strerror(errno) << endl;
        exit(1);
    }

    int index = 0; // table index
    int count = 0; // to prevent program on going forerver

    cout << "Consumer started..." << endl;

    while (true) {
        sem_wait(full);
        sem_wait(mutex);

        // Critical Section
        int item = table[index];
        cout << "Consumer consumed " << item << " from index " << index << endl;
        index = (index + 1) % TABLE_SIZE;
        ++count;
        // Critical section

        sem_post(mutex);
        sem_post(empty);
        sleep(2); // simulate consumption

        if (count >= 10) 
            break;
    }

    munmap(table, TABLE_SIZE * sizeof(int));
    close(shm_fd);
}
