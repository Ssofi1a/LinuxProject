#include "common.h"
const int add = 0;
const int minus = 1;
const int multiply = 2;
const int divide = 3;

class Task
{
public:
  int arg_1, arg_2, type, result;
  Task(int arg_1, int arg_2, int type)
  {
    this->arg_1 = arg_1;
    this->arg_2 = arg_2;
    this->type = type;
  }

  void make_operation()
  {
    switch (this->type)
    {
    case add:
      result = this->arg_1 + this->arg_2;
      break;
    case minus:
      result = this->arg_1 - this->arg_2;
      break;
    case multiply:
      result = this->arg_1 * this->arg_2;
      break;
    case divide:
      if (this->arg_2 == 0)
      {
        exit(EXIT_FAILURE);
      }
      result = this->arg_1 / this->arg_2;
      break;
    default:
      break;
    }
  }
};

int main()
{

  sem_t *res_sem = sem_open("/res_semaphore", O_RDWR, 0, 0);
  sem_t *req_sem = sem_open("/req_semaphore", O_RDWR, 0, 0);
  int shm_size = 4 * sizeof(Task *);

  if (req_sem == SEM_FAILED || res_sem == SEM_FAILED)
  {
    std::cerr << "Couldn't create a semaphore";
  }

  int fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    std::cerr << "Failed to open shared memory\n";
  }

  if (ftruncate(fd, shm_size) == -1)
  {
    std::cerr << "Couldn't truncate the memory\n";
  }

  Task *shm_start = static_cast<Task *>(mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
  if (shm_start == MAP_FAILED)
  {
    std::cerr << "Couldn't map the shared memory";
  }

  std::cout << "0: Addition" << std::endl
            << "1: Substraction" << std::endl
            << "2: Multiplication" << std::endl
            << "3: Division" << std::endl;

  int userID;
  std::cin >> userID;

  int firstNumber, secondNumber;
  std::cout << "Enter numbers: " << std::endl;
  std::cin >> firstNumber >> secondNumber;
  shm_start->type = userID;
  shm_start->arg_1 = firstNumber;
  shm_start->arg_2 = secondNumber;
  sem_post(req_sem);
  sem_wait(res_sem);

  int result = shm_start->result;
  std::cout << "The result is: " << result << std::endl;
  if (munmap(shm_start, shm_size) == -1)
  {
    std::cerr << "Couldn't unmap the shared memory\n";
    return 1;
  }

  if (shm_unlink(SHARED_MEMORY_NAME) == -1)
  {
    std::cerr << "Couldn't unlink the shared memory\n";
    return 1;
  }

  return 0;
}
