#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <cstdlib>
#include <cerrno>
#include <limits.h>
#include <pthread.h>
#include <vector>
#include <queue>
#include <signal.h>

const char *SHARED_MEMORY_NAME = "/shared_memory";
