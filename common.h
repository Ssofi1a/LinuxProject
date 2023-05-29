#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <cerrno>
#include <cstdlib>
#include <limits.h>

const char *SHARED_MEMORY_NAME = "/shared_memory";
