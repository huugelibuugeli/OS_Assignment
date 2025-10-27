# OS_Assignment

This project demonstrates a simple producer/consumer example using POSIX shared memory and semaphores. Implemented in C++.

Primitives used:

- POSIX shared memory (`shm_open`, `mmap`) to share an integer table between processes.
- POSIX named semaphores (`sem_open`, `sem_wait`, `sem_post`, `sem_unlink`) to synchronize access.

Main semaphore commands used:

- `sem_wait(sem)` — decrement (P). If the value is zero, block until it is > 0 and then decrement
- `sem_post(sem)` — increment (V). Atomically increases the semaphore and wakes one waiting process (if any)

Prereqs
------------

- Linux system with a modern glibc toolchain
- C++ compiler supporting C++11

Compilation and running

```bash
$ gcc producer.cpp -pthread -lrt -o producer
$ gcc consumer.cpp -pthread -lrt -o consumer
$ ./producer & ./consumer &
```

Console output should show that producer is starting to produce items and that consumer is starting to consume them. The program is hardcoded to only running 10 iterations. 



